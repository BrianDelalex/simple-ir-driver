/*****************************************************************************\
**  This file is part of simple-ir-driver project, and is made available under
**  ther terms of the GNU AFFERO General Public License version 3.
**
**  Copyright (C) 2025 - Brian DELALEX-FONDU
**
\*****************************************************************************/

#include "irdriver_circular_buffer.h"

#ifdef __KERNEL__
#   include <linux/string.h>
#else
#   include <string.h>
#endif//!__KERNEL__

# define INCREMENT_OFF(off) \
        off = off + 1 >= IRDRIVER_MAX_CONCURRENT_SIGNALS ? 0 : off + 1

void irdriver_circular_buffer_add_entry(
    struct irdriver_circular_buffer *buffer, struct ir_data *data)
{
    uint8_t next = buffer->in_offs + 1;

    buffer->entries[buffer->in_offs] = data;
    if (next >= IRDRIVER_MAX_CONCURRENT_SIGNALS) {
        next = 0;
        buffer->full = true;
    }

    buffer->in_offs = next;

}

struct ir_data *irdriver_circular_buffer_read(struct irdriver_circular_buffer *buffer)
{
    struct ir_data *data = buffer->entries[buffer->out_offs];

    buffer->entries[buffer->out_offs] = NULL;
    
    INCREMENT_OFF(buffer->out_offs);

    return data;
}

void irdriver_circular_buffer_init(struct irdriver_circular_buffer *buffer)
{
    memset(buffer, 0, sizeof(struct irdriver_circular_buffer));
}