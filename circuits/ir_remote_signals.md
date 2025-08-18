# Summary
This file describes the procedure followed to find the different signals that the IR remote control sends for each button.

# Components
IR receiver - [Vishay Semiconductors TSOP34838](https://www.vishay.com/docs/82489/tsop322.pdf)  
IR remote control - Any IR remote control can be used as long as, its signal is detected by the receiver.

# Circuit
![IR Remote signals finder diagram](https://github.com/BrianDelalex/simple-ir-driver/blob/main/media/ir_signals.png)  
![IR remote signals finder schema](https://github.com/BrianDelalex/simple-ir-driver/blob/main/media/ir_signals_schem.png)  

Using this circuit, we can receive the signals from the sensor's DATA pin into the digital pin 2 of the Arduino. 

# Arduino Sketches
Within the rc_signal_tools directory of this repository reside two Arduino sketches. Both sketchs come from [Adafruit Learning System Guide](https://github.com/adafruit/Adafruit_Learning_System_Guides) and have been modified to meet the needs of this project.

## Raw_IR
The IR remote control sends pulses to the receiver. It is by using those pulses that we can distinguish which button has been pressed.  
Using this principle, this program will simply wait for a signal and as soon as the signal starts it will count the time that the D2 pin remains in the HIGH and LOW states.  
The program then prints the result to the serial port of the Arduino which we can read from Arduino IDE. See example below.
```

Received: 

OFF 	ON
25716 usec, 9320 usec
4620 usec, 620 usec
540 usec, 620 usec
540 usec, 640 usec
540 usec, 600 usec
560 usec, 620 usec
540 usec, 620 usec
540 usec, 620 usec
560 usec, 600 usec
...
```
> We ignore the first OFF pulse (this is the time waited before the signal start) and the last ON pulse.

Using those values, the program also generates a C array (example below) that will be useful to determine which signal we have received later. 
```
int IRsignal[] = {
// ON, OFF (in 10's of microseconds)
    932, 462,
    62, 54,
    62, 54,
    64, 54,
    60, 56,
    62, 54,
    62, 54,
    62, 56,
    60, 56,
    60, 170,
    62, 168,
    ...
};
```

So I have generated the C arrays for all the buttons on the remote control and put them in the file under `irdriver/irsignal.h`.

## IR_Commander
This sketch uses C arrays to check the incoming signals and determine which button has been pressed. It uses the file generated using Raw_IR (`irsignal.h`).  
The program registers every pulse within a signal and then compares it to one of the arrays. There is a constant variable, `FUZZINESS` to set the margin of error that we allow.

Using this program, I've been able to recognize every button on the remote control. Since the `irsignal.h` file use C language we will be able to use it in the driver later.

# Results
Using this circuit and those two Arduino sketches, I've been able to find a way to distinguish every signal that the remote control can send.