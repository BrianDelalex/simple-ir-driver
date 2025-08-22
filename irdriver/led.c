/*****************************************************************************\
**  This file is part of simple-ir-driver project, and is made available under
**  ther terms of the GNU AFFERO General Public License version 3.
**
**  Copyright (C) 2025 - Brian DELALEX-FONDU
**
\*****************************************************************************/

#include <linux/cdev.h>
#include <linux/gpio.h>
#include <linux/timer.h>
#include <linux/jiffies.h>

#include "irdriver.h"

static struct timer_list led_timer;

# define BLINK_DURATION_MS 1000

void led_timer_callback(struct timer_list * data)
{
    gpio_set_value(GPIO_LED, 0);
}

void blink_led(void)
{
    gpio_set_value(GPIO_LED, 1);
    mod_timer(&led_timer, jiffies + msecs_to_jiffies(BLINK_DURATION_MS));
}

void init_led_timer(void)
{
    timer_setup(&led_timer, led_timer_callback, 0);
}

void release_led_timer(void)
{
    del_timer(&led_timer);
}