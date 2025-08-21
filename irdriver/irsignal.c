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

#include "irsignal.h"
#include "irdriver.h"

int read_ir_signal(struct ir_data *ir_data)
{
    int currentpulse = 0;

    while (1) {
        int highpulse = 0;
        int lowpulse = 0;
        while (gpio_get_value(GPIO_RECEIVER_DATA)) {
            highpulse++;
            udelay(RESOLUTION);

            if (highpulse >= PULSE_TIMEOUT || currentpulse == MAX_PULSES) {
                return currentpulse;
            }
        }

        ir_data->pulses[currentpulse][0] = highpulse;

        while (gpio_get_value(GPIO_RECEIVER_DATA) == 0) {
            lowpulse++;
            udelay(RESOLUTION);
            if (highpulse >= PULSE_TIMEOUT || currentpulse == MAX_PULSES) {
                return currentpulse;
            }
        }

        ir_data->pulses[currentpulse][1] = lowpulse;

        currentpulse++;
    }
}

irsignal_t identify_signal(struct ir_data *ir_data, int pulses)
{
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

void process_irsignal(irsignal_t sig)
{
    switch (sig) {
    case VOL_MINUS:
        PDEBUG("Signal catch: VOL_MINUS\n");
        break;
    case PLAY:
        PDEBUG("Signal catch: PLAY\n");
        break;
    case VOL_PLUS:
        PDEBUG("Signal catch: VOL_PLUS\n");
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