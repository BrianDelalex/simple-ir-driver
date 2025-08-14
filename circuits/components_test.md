# Summary
This file describe the process followed to test the IR components (IR receiver and IR remote control).

# Components
IR receiver - [Vishay Semiconductors TSOP34838](https://www.vishay.com/docs/82489/tsop322.pdf)
IR remote control - Any IR remote control can be used as long as, its signal is detected by the receiver.

# Circuit
![Test components diagram](https://github.com/BrianDelalex/simple-ir-driver/blob/main/media/test_ir.png)  
![Test components schema](https://github.com/BrianDelalex/simple-ir-driver/blob/main/media/test_ir_schem.png)

The circuit above has been used to test the components.  
By default, the DATA pin on the IR receiver output is equal to VCC+0.3 V. As we supply 5V to VCC, the DATA pin outputs 5.3V so the LED is off. When the IR receiver receives a signal it outputs 0V to the DATA pin, the LED will then be ON.
We use this simple principle to make sure that the IR remote control sends a signal and that the IR receiver receives it. The LED should blink whenever a button of the IR remote control is pressed.

> Any 5V supply can be used as VCC. For example, I used the 5V output from an Arduino for my tests.

# Result
I assembled the electronic circuit using an Arduino as power supply.  
I have tested all the buttons on the remote control. For each button the LED lit up, so I concluded that the components are working properly.