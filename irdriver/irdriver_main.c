/*****************************************************************************\
**  This file is part of simple-ir-driver project, and is made available under
**  ther terms of the GNU AFFERO General Public License version 3.
**
**  Copyright (C) 2025 - Brian DELALEX-FONDU
**
\*****************************************************************************/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <linux/kthread.h>
#include <linux/completion.h>
#include <linux/mutex.h>

#include "irdriver.h"
#include "irsignal.h"
#include "irdriver_circular_buffer.h"
#include "led.h"

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("BrianDelalex");

int irdriver_major = 0;
int irdriver_minor = 0;

dev_t dev = 0;
struct irdriver_dev irdriver_device;
static struct class *irdriver_class;

unsigned int gpio_irq_number;


static struct task_struct *signal_hdl_thread;
struct irdriver_circular_buffer *circular_buffer;
struct mutex circular_buffer_mtx;
struct completion signal_recv_completion;


/*************************
*   IRQ HANDLERS
**************************/

static irqreturn_t ir_signal_handler(int irq, void *dev)
{
    unsigned long flags = 0;
    struct ir_data* ir_data;

    local_irq_save(flags);
    PDEBUG("IRQ Handler called.\n");

    ir_data = kmalloc(sizeof(struct ir_data), GFP_KERNEL);
    if (ir_data == NULL) {
        PERR("ir_signal_handler: Out of memory! Can't process signal.");
        goto restore_irq;
    }

    memset(ir_data, 0, sizeof(struct ir_data));

    read_ir_signal(ir_data);

    PDEBUG("Catch %d pulse signal\n", ir_data->pulses_count);
    mutex_lock(&circular_buffer_mtx);
    irdriver_circular_buffer_add_entry(circular_buffer, ir_data);
    mutex_unlock(&circular_buffer_mtx);
    complete(&signal_recv_completion);

restore_irq:
    local_irq_restore(flags);
    return IRQ_HANDLED;
}

/*************************
*   FILE OPERATIONS 
**************************/

int irdriver_open(struct inode *inode, struct file* filp)
{
    PDEBUG("open called.\n");

    return 0;
}

int irdriver_release(struct inode *inode, struct file* filp)
{
    PDEBUG("release called.\n");

    return 0;
}

long irdriver_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    long retval = -EINVAL;

    PDEBUG("ioctl called.\n");
    gpio_set_value(GPIO_LED, (int) HIGH);
    mdelay(2000);
    gpio_set_value(GPIO_LED, (int) LOW);

    retval = 0;

    return retval;
}

int init_gpio(int index, const char* lbl, direction_t direction)
{
    if (!gpio_is_valid(index)) {
        PERR("GPIO %d is not valid\n", index);
        return -1;
    }

    if (gpio_request(index, lbl) < 0) {
        PERR("GPIO %d request failed\n", index);
        return -1;
    }

    if (direction == OUTPUT) {
        gpio_direction_output(index, (int) LOW);
    } else {
        gpio_direction_input(index);
    }

    // Exporting for test purpose
    gpio_export(index, false);

    return 0;
}

struct file_operations irdriver_fops = {
    .owner = THIS_MODULE,
    .open = irdriver_open,
    .release = irdriver_release,
    .unlocked_ioctl = irdriver_ioctl,
};

static int irdriver_setup_cdev(struct irdriver_dev *dev)
{
    int err;
    int devno = MKDEV(irdriver_major, irdriver_minor);

    cdev_init(&dev->cdev, &irdriver_fops);
    dev->cdev.owner = THIS_MODULE;
    dev->cdev.ops = &irdriver_fops;
    err = cdev_add(&dev->cdev, devno, 1);
    if (err) {
        PERR("Error %d adding irdriver cdev.\n", err);
    }
    return err;
}

static int __init irdriver_init(void)
{
    PDEBUG("irdriver init.\n");

    int rc;

    rc = alloc_chrdev_region(&dev, irdriver_minor, 1, "irdriver");
    irdriver_major = MAJOR(dev);
    if (rc < 0) {
        PERR("Can't get major!\n");
        return rc;
    }
    memset(&irdriver_device, 0, sizeof(struct irdriver_dev));

    if (irdriver_setup_cdev(&irdriver_device) != 0)
        goto r_chrdev;

    if (IS_ERR(irdriver_class = class_create(THIS_MODULE, "irdriver_class"))) {
        PERR("Cannot create the struct class\n");
        goto r_class;
    }

    if (IS_ERR(device_create(irdriver_class, NULL, dev, NULL, "irdriver"))) {
        PERR("Cannot create the device\n");
        goto r_device;
    }
    
    if (init_gpio(GPIO_RECEIVER_DATA, GPIO_RECEIVER_DATA_LBL, INPUT) != 0) {
        goto r_gpio_receiver_data;
    }

    if (init_gpio(GPIO_LED, GPIO_LED_LBL, OUTPUT) != 0) {
        goto r_gpio_led;
    }

    gpio_irq_number = gpio_to_irq(GPIO_RECEIVER_DATA);
    if (request_irq(gpio_irq_number,
                    (void *)ir_signal_handler,
                    IRQF_TRIGGER_LOW | IRQF_IRQPOLL, 
                    "irdriver", 
                    NULL)) {
        PERR("cannot register IRQ!");
        goto r_gpio_led;
    }

    init_led_timer();

    circular_buffer = kmalloc(sizeof(struct irdriver_circular_buffer), GFP_KERNEL);
    if (circular_buffer == NULL) {
        PERR("Out of memory! Can't alloc circular_buffer\n");
        goto r_led_timer; 
    }

    mutex_init(&circular_buffer_mtx);

    init_completion(&signal_recv_completion);

    signal_hdl_thread = kthread_run(thread_signal_handling, NULL, "irdriver_thread");
    if (!signal_hdl_thread) {
        PERR("kthread creation failed.\n");
        goto r_circular_buffer;
    }


    return 0;
r_circular_buffer:
    kfree(circular_buffer);
r_led_timer:
    release_led_timer();
    free_irq(gpio_irq_number, NULL);
r_gpio_led:
    gpio_free(GPIO_LED);
r_gpio_receiver_data:
    gpio_free(GPIO_RECEIVER_DATA);
r_device:
    device_destroy(irdriver_class, dev);
r_class:
    class_destroy(irdriver_class);
r_chrdev:
    unregister_chrdev_region(dev, 1);
    return -1;
}

static void __exit irdriver_exit(void)
{
    PDEBUG("irdriver exit.\n");

    kfree(circular_buffer);

    release_led_timer();

    free_irq(gpio_irq_number, NULL);

    gpio_unexport(GPIO_LED);
    gpio_free(GPIO_LED);
    
    gpio_unexport(GPIO_RECEIVER_DATA);
    gpio_free(GPIO_RECEIVER_DATA);

    device_destroy(irdriver_class, dev);
    class_destroy(irdriver_class);
    unregister_chrdev_region(dev, 1);
}

module_init(irdriver_init);
module_exit(irdriver_exit);