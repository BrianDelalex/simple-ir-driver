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
enum {
 BTN_VOL_MINUS = 0,
 BTN_PLAY,     
 BTN_VOL_PLUS, 
 BTN_SETUP,    
 BTN_PREVIOUS, 
 BTN_STOP,     
 BTN_CHNL_MINU,
 BTN_ENTER,    
 BTN_CHNL_PLUS,
 BTN_ZERO,      
 BTN_NEXT,      
 BTN_BACK,      
 BTN_ONE,       
 BTN_TWO,       
 BTN_THREE,     
 BTN_FOUR,      
 BTN_FIVE,      
 BTN_SIX,       
 BTN_SEVEN,     
 BTN_EIGHT,     
 BTN_NINE,
};      

#endif//!IRDRIVER_API_H