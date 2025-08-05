# NCG-2025

## TMC5272 Driver

The Trinamic TMC5272 driver provides a convenient API to control motor movement via position & velocity control. 

The driver also establishes SPI1 communications between the MAX32650 FTHR board and a single TMC5272. Initialization configures the registers of the TMC5272 for driving Motor 0.

An example sketch is provided in `main.c`. The sketch begins by rotating Motor 0 through a deterministic set of movements in both position and velocity control modes. Pressing either pushbutton (PB1 or PB2) causes the motor to rotate counter/clockwise. 