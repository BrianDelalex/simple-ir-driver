/*****************************************************************************\
**  This file is part of simple-ir-driver project, and is made available under
**  ther terms of the GNU AFFERO General Public License version 3.
**
**  Copyright (C) 2025 - Brian DELALEX-FONDU
**
\*****************************************************************************/

#ifndef IR_SIGNAL_H
#define IR_SIGNAL_H

#ifdef ARDUINO
# define SIGNAL_TYPE(var_name) const int var_name[] PROGMEM
#else
# define SIGNAL_TYPE(var_name) const int var_name[]
#endif //!ARDUINO

typedef enum {
    VOL_MINUS,
    PLAY,
    VOL_PLUS,
    SETUP,
    PREVIOUS,
    STOP_MODE,
    CHNL_MINUS,
    ENTER,
    CHNL_PLUS,
    CHNL_ZERO,
    NEXT,
    BACK,
    CHNL_ONE,
    CHNL_TWO,
    CHNL_THREE,
    CHNL_FOUR,
    CHNL_FIVE,
    CHNL_SIX,
    CHNL_SEVEN,
    CHNL_EIGHT,
    CHNL_NINE,
    UNKNOW
} irsignal_t;

struct signal_pulses_s {
    const int *pulses;
    int pulses_count;
    irsignal_t code;
    bool valid;
};

# define PULSE_TIMEOUT  65000
# define MAX_PULSES     50
# define RESOLUTION     20
# define FUZZINESS      20

struct ir_data {
    int pulses[MAX_PULSES][2];
};

int read_ir_signal(struct ir_data *ir_data);
irsignal_t identify_signal(struct ir_data *ir_data, int pulses);
struct signal_pulses_s* get_signal_pulses_array(int* size);
void process_irsignal(irsignal_t sig);


#endif //!IR_SIGNAL_H