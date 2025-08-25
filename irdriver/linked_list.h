/*****************************************************************************\
**  This file is part of simple-ir-driver project, and is made available under
**  ther terms of the GNU AFFERO General Public License version 3.
**
**  Copyright (C) 2025 - Brian DELALEX-FONDU
**
\*****************************************************************************/

#ifndef IRDRIVER_LINKED_LIST_H
# define IRDRIVER_LINKED_LIST_H

#ifdef __KERNEL__
#   define FREE(ptr) kfree(ptr)
#else
#   define FREE(ptr) free(ptr)
#endif//__KERNEL__

# define LL_ADD(s_name, head, next, entry)   \
    struct s_name *ptr = head;                \
    while (ptr->next)               \
        ptr = ptr->next;            \
    ptr->next = entry;

# define LL_REMOVE(type, head, next, data_name, data)  \
    struct type *ptr = head;                        \
    struct type *previous = NULL;                   \
    while (ptr) {                     \
        if (ptr->data_name == data) {       \
            if (!previous) {                \
                ptr = ptr->next;            \
                FREE(head);                 \
                return ptr;                 \
            }                               \
            previous->next = ptr->next;     \
            FREE(ptr);                      \
            return head;                    \
        }                                   \
        previous = ptr;                     \
        ptr = ptr->next;                    \
    }                                       \
    return head;                            \

# define LL_EXISTS(s_name, head, next, data_name, data)   \
    struct s_name *ptr = head;                                    \
    while (ptr) {                                 \
        if (ptr->data_name == data)                     \
            return true;                                \
        ptr = ptr->next;                                \
    }                                                   \
    return false;                                       \

# define LL_DUMP(s_name, head, next, data_name, fmt) \
    struct s_name *ptr = head;                      \
    while (ptr) {                                   \
        PDEBUG(fmt "%d\n", ptr->data_name);                \
        ptr = ptr->next;                            \
    }                                               \

#endif//!IRDRIVER_LINKED_LIST_H