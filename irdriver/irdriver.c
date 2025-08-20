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
MODULE_LICENSE("Dual BSD/GPL");

# include "irdriver.h"

# define GPIO_RECEIVER_DATA     (17)
# define GPIO_LED               (24)
# define GPIO_RECEIVER_DATA_LBL "GPIO_17"
# define GPIO_LED_LBL           "GPIO_24"

typedef enum {
    OUTPUT,
    INPUT
} direction_t;

typedef enum {
    LOW = 0,
    HIGH
} GPIO_STATE;

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

static int __init irdriver_init(void)
{
    PDEBUG("irdriver init.\n");

    
    if (init_gpio(GPIO_RECEIVER_DATA, GPIO_RECEIVER_DATA_LBL, INPUT) != 0) {
        return -1;
    }

    if (init_gpio(GPIO_LED, GPIO_LED_LBL, OUTPUT) != 0) {
        goto r_gpio_receiver_data;
    }

    // Testing GPIO to validate DoD of issue #5.
    PDEBUG("GPIO 17 value = %d\n", gpio_get_value(GPIO_RECEIVER_DATA));
    gpio_set_value(GPIO_LED, (int) HIGH);
    mdelay(2000);
    gpio_set_value(GPIO_LED, (int) LOW);

    return 0;

r_gpio_led:
    gpio_free(GPIO_LED);
r_gpio_receiver_data:
    gpio_free(GPIO_RECEIVER_DATA);
    return -1;
}

static void __exit irdriver_exit(void)
{
    PDEBUG("irdriver exit.\n");

    gpio_unexport(GPIO_LED);
    gpio_free(GPIO_LED);
    
    gpio_unexport(GPIO_RECEIVER_DATA);
    gpio_free(GPIO_RECEIVER_DATA);
}

module_init(irdriver_init);
module_exit(irdriver_exit);