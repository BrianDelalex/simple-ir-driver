/*****************************************************************************\
**  This file is part of simple-ir-driver project, and is made available under
**  ther terms of the GNU AFFERO General Public License version 3.
**
**  Copyright (C) 2025 - Brian DELALEX-FONDU
**
\*****************************************************************************/

#include <linux/init.h>
#include <linux/module.h>
MODULE_LICENSE("Dual BSD/GPL");

# include "irdriver.h"

static int __init irdriver_init(void)
{
    PDEBUG("irdriver init.\n");
    return 0;
}

static void __exit irdriver_exit(void)
{
    PDEBUG("irdriver exit.\n");
}

module_init(irdriver_init);
module_exit(irdriver_exit);