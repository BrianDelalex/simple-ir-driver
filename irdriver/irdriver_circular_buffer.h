/*****************************************************************************\
**  This file is part of simple-ir-driver project, and is made available under
**  ther terms of the GNU AFFERO General Public License version 3.
**
**  Copyright (C) 2025 - Brian DELALEX-FONDU
**
\*****************************************************************************/

#ifndef IRDRIVER_CIRCULAR_BUFFER_H
#define IRDRIVER_CIRCULAR_BUFFER_H

#ifdef __KERNEL__
#   include <linux/types.h>
#else
#   include <stdbool.h>
#   include <stdint.h>
#endif//!__KERNEL__

#include "irsignal.h"

# define IRDRIVER_MAX_CONCURRENT_SIGNALS 10

struct irdriver_circular_buffer {
    struct ir_data *entries[IRDRIVER_MAX_CONCURRENT_SIGNALS];

    uint8_t in_offs;
    uint8_t out_offs;

    bool full;
};

void irdriver_circular_buffer_add_entry(struct irdriver_circular_buffer *buffer, struct ir_data *data);
struct ir_data *irdriver_circular_buffer_read(struct irdriver_circular_buffer *buffer);
void irdriver_circular_buffer_init(struct irdriver_circular_buffer *buffer);

#endif //!IRDRIVER_CIRCULAR_BUFFER_H