/*****************************************************************************\
**  This file is part of simple-ir-driver project, and is made available under
**  ther terms of the GNU AFFERO General Public License version 3.
**
**  Copyright (C) 2025 - Brian DELALEX-FONDU
**
\*****************************************************************************/

#ifndef IRDRIVER_H
# define IRDRIVER_H

#undef PDEBUG
#ifdef IRDRIVER_DEBUG
#   ifdef __KERNEL__
#       define PDEBUG(fmt, args...) printk( KERN_DEBUG "irdriver: " fmt, ## args)
#   else
#       define PDEBUG(fmt, args...) fprintf(stderr, fmt, ## args)
#   endif //!__KERNEL__
#else
#   define PDEBUG(fmt, args...) /* not debugging: nothing */
#endif //!IRDRIVER_DEBUG

#ifdef __KERNEL__
#   define PERR(fmt, args...) printk( KERN_ERR "irdriver: [ERROR] " fmt, ## args)
#else
#   define PERR(fmt, args...) fprintf(stderr, fmt, ## args)
#endif //!__KERNEL__


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

struct irdriver_dev {
    struct cdev cdev;
};

# define MAX_PULSES 50

struct ir_data {
    int pulses[MAX_PULSES][2];
};

#endif //!IRDRIVER_H