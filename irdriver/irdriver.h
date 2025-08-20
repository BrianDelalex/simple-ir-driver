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

#endif //!IRDRIVER_H