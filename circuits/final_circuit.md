# Summary
This file describes the final circuit that is meant to be used with the driver.

# Components
Raspberry Pi 4 Model B (8GB RAM)  
IR receiver - [Vishay Semiconductors TSOP34838](https://www.vishay.com/docs/82489/tsop322.pdf)  
IR remote control - Any IR remote control can be used as long as, its signal is detected by the receiver.  
LED red 5mm  
220 Ohm resistor.

# Circuit
![Final circuit diagram](https://github.com/BrianDelalex/simple-ir-driver/blob/main/media/final_circuit.png)  
![Final circuit schema](https://github.com/BrianDelalex/simple-ir-driver/blob/main/media/final_circuit_schem.png)  
![Final circuit picture](https://github.com/BrianDelalex/simple-ir-driver/blob/main/media/final_circuit_picture.jpg)  

The IR receiver will be supplied through a 5V output pin from the Raspberry Pi. The IR receiver's DATA pin will be wired to the Raspberry Pi's GPIO17 pin. The GPIO17 pin will be configured as an input by the driver to read the information from the receiver.  
The GPIO24 pin from the Raspberry Pi will be configured as an output and will be wired to the LED through a 220-ohm resistor.

# Result
The circuit has been assembled. The driver can now interact with the different components.