/*****************************************************************************\
**  This file is part of simple-ir-driver project, and is made available under
**  ther terms of the GNU AFFERO General Public License version 3.
**
**  Copyright (C) 2025 - Brian DELALEX-FONDU
**
\*****************************************************************************/


#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/cdev.h>
#include <linux/kthread.h>
#include <linux/completion.h>
#include <linux/mutex.h>
#include <asm/siginfo.h>
#include <linux/sched.h>

#include "irsignal.h"
#include "irdriver.h"
#include "irdriver_circular_buffer.h"
#include "led.h"
#include "irdriver_api.h"

extern struct irdriver_circular_buffer *circular_buffer;
extern struct mutex circular_buffer_mtx;
extern struct completion signal_recv_completion;

extern struct registered_app *registered_app_head;

void read_ir_signal(struct ir_data *ir_data)
{
    int currentpulse = 0;

    while (1) {
        int highpulse = 0;
        int lowpulse = 0;
        while (gpio_get_value(GPIO_RECEIVER_DATA)) {
            highpulse++;
            udelay(RESOLUTION);

            if (highpulse >= PULSE_TIMEOUT || currentpulse == MAX_PULSES) {
                ir_data->pulses_count = currentpulse;
                return;
            }
        }

        ir_data->pulses[currentpulse][0] = highpulse;

        while (gpio_get_value(GPIO_RECEIVER_DATA) == 0) {
            lowpulse++;
            udelay(RESOLUTION);
            if (highpulse >= PULSE_TIMEOUT || currentpulse == MAX_PULSES) {
                ir_data->pulses_count = currentpulse;
                return;
            }
        }

        ir_data->pulses[currentpulse][1] = lowpulse;

        currentpulse++;
    }
}

irsignal_t identify_signal(struct ir_data *ir_data)
{
    int pulses = ir_data->pulses_count;
    int count = 0;
    int pulses_arr_size;
    struct signal_pulses_s* pulses_arr = get_signal_pulses_array(&pulses_arr_size);

    if (pulses_arr == NULL) {
        return UNKNOW;
    }

    while (count < pulses - 1) {
        int oncode = ir_data->pulses[count][1] * RESOLUTION / 10;
        int offcode = ir_data->pulses[count + 1][0] * RESOLUTION / 10;
        int arr_index = 0;

        while (arr_index < pulses_arr_size) {
            if (pulses_arr[arr_index].valid == false) {
                arr_index++;
                continue;
            }
            if (count * 2 + 1 >= pulses_arr[arr_index].pulses_count) {
                arr_index++;
                continue;
            }
            int comp_on = pulses_arr[arr_index].pulses[count * 2];
            int comp_off = pulses_arr[arr_index].pulses[count * 2 + 1];

            if (abs(oncode - comp_on) > (comp_on * FUZZINESS / 100)) {
                pulses_arr[arr_index].valid = false;
            }
            if (abs(offcode - comp_off) > (comp_off * FUZZINESS / 100)) {
                pulses_arr[arr_index].valid = false;
            }
            arr_index++;
        }
        count++;
    }

    int index = 0;
    while (index < pulses_arr_size) {
        if (pulses_arr[index].valid) {
            int rc = pulses_arr[index].code;
            kfree(pulses_arr);
            return rc;
        }
        index++;
    }

    kfree(pulses_arr);
    return UNKNOW;
}

void signal_registered_app(int code)
{
    struct registered_app* app = registered_app_head;

    while (app != NULL) {
        struct kernel_siginfo info;
        struct task_struct *t;

        memset(&info, 0, sizeof(struct kernel_siginfo));
        info.si_signo = SIGPOLL;
        info.si_code = SI_QUEUE;
        info.si_int = code;

        t = pid_task(find_vpid(app->pid), PIDTYPE_PID);
        if (t == NULL) {
            PERR("signal_registered_app: Can't find task with pid %d\n", app->pid);
        } else {
            if (send_sig_info(SIGPOLL, &info, t) < 0 )  {
                PERR("Unable to send signal to %d\n", app->pid);
            }
        }

        app = app->next;
    }
}

void process_irsignal(irsignal_t sig)
{
    switch (sig) {
    case VOL_MINUS:
        PDEBUG("Signal catch: VOL_MINUS\n");
        signal_registered_app(BTN_VOL_MINUS);
        break;
    case PLAY:
        PDEBUG("Signal catch: PLAY\n");
        signal_registered_app(BTN_PLAY);
        break;
    case VOL_PLUS:
        PDEBUG("Signal catch: VOL_PLUS\n");
        signal_registered_app(BTN_VOL_PLUS);
        break;
    case SETUP:
        PDEBUG("Signal catch: SETUP\n");
        break;
    case PREVIOUS:
        PDEBUG("Signal catch: PREVIOUS\n");
        break;
    case STOP_MODE:
        PDEBUG("Signal catch: STOP_MODE\n");
        break;
    case CHNL_MINUS:
        PDEBUG("Signal catch: CHNL_MINUS\n");
        break;
    case ENTER:
        PDEBUG("Signal catch: ENTER\n");
        break;
    case CHNL_PLUS:
        PDEBUG("Signal catch: CHNL_PLUS\n");
        break;
    case CHNL_ZERO:
        PDEBUG("Signal catch: CHNL_ZERO\n");
        break;
    case NEXT:
        PDEBUG("Signal catch: NEXT\n");
        break;
    case BACK:
        PDEBUG("Signal catch: BACK\n");
        break;
    case CHNL_ONE:
        PDEBUG("Signal catch: CHNL_ONE\n");
        break;
    case CHNL_TWO:
        PDEBUG("Signal catch: CHNL_TWO\n");
        break;
    case CHNL_THREE:
        PDEBUG("Signal catch: CHNL_THREE\n");
        break;
    case CHNL_FOUR:
        PDEBUG("Signal catch: CHNL_FOUR\n");
        break;
    case CHNL_FIVE:
        PDEBUG("Signal catch: CHNL_FIVE\n");
        break;
    case CHNL_SIX:
        PDEBUG("Signal catch: CHNL_SIX\n");
        break;
    case CHNL_SEVEN:
        PDEBUG("Signal catch: CHNL_SEVEN\n");
        break;
    case CHNL_EIGHT:
        PDEBUG("Signal catch: CHNL_EIGHT\n");
        break;
    case CHNL_NINE:
        PDEBUG("Signal catch: CHNL_NINE\n");
        break;
    default:
        PDEBUG("Unknow Signal\n");
    }
}

int thread_signal_handling(void *data)
{
    struct ir_data *ir_data;
    while (1) {
        wait_for_completion(&signal_recv_completion);
        PDEBUG("Completion !\n");
        if (kthread_should_stop()) {
            return 0;
        }
        mutex_lock(&circular_buffer_mtx);
        ir_data = irdriver_circular_buffer_read(circular_buffer);
        mutex_unlock(&circular_buffer_mtx);

        if (!ir_data)
            continue;
        int i = 0;
        PDEBUG("ON, OFF\n");
        while (i < ir_data->pulses_count) {
            PDEBUG("%d, %d", ir_data->pulses[i][1] * RESOLUTION / 10, ir_data->pulses[i + 1][0] * RESOLUTION / 10);
            i++;
        }

        irsignal_t sig = identify_signal(ir_data);
        if (sig != UNKNOW) {
            blink_led();
        }
        kfree(ir_data);
        process_irsignal(sig);
        reinit_completion(&signal_recv_completion);
    }
}