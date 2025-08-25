/*****************************************************************************\
**  This file is part of simple-ir-driver project, and is made available under
**  ther terms of the GNU AFFERO General Public License version 3.
**
**  Copyright (C) 2025 - Brian DELALEX-FONDU
**
\*****************************************************************************/

#ifndef IRDRIVER_API_H
# define IRDRIVER_API_H

// IOCTL commands
# define CMD_REGISTER 1
# define CMD_UNREGISTER 2


// Remote control buttons
# define SIGNAL_IR      SIGRTMIN + 1
# define BTN_VOL_MINUS  1
# define BTN_PLAY       2
# define BTN_VOL_PLUS   3

#endif//!IRDRIVER_API_H