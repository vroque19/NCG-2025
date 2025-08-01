# NCG-2025
TODO

## TMC SPI Driver

The Trinamic TMC5272 SPI driver establishes the communications protocol on SPI1 between the MAX32650 FTHR board and a single TMC5272.

The driver configures the registers of the TMC5272 to drive Motor 0. Pressing either pushbutton (PB1 or PB2) causes the motor to rotate counter/clockwise. 