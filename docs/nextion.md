# NX4827P043-011 Touchscreen Interface

## Overview
The display used for this system is from the Nextion Intelligent Series, combining a resistive touch display with a powerful onboard processor. It features 128MB of flash memory and 512 KB of RAM, providing ample resources for complex user interfaces. The Nextion MCU communicates with the MAX32650fthr via a standard TTL serial connection, using a straightforward ASCII text-based instruction set. This makes the interaction between the microcontroller and the display simple and highly effective.

## TTL (Transistor-Transistor Logic)
TTL is the communication protocol used by this display.  TTl is a form of **UART serial communication** with the operating logic level being **5V** and baud rate **9600bps**.

## Nextion Editor
The Nextion editor provides a user-friendly GUI to setup the display with drag-and-drop components. A particularly useful feature is its **built-in debug mode**, which enables you to test the functionality and communication of your project directly on a PC before uploading it to the physical display. This was invaluable for verifying that the expected ASCII commands and data were being correctly sent and received between the display and the MAX32650.
### How to flash a new design
1. Download the Nextion Editor Software
2. Open the Nextion Editor
3. Choose the display that you are working with
4. Add components, pages, and logic to create your design
5. Plug in your MicroSD card
6. Select `File` -> `Output as *.tft file`
7. Drag the *.tft file to the MicroSD card
8. Eject from PC and insert into Nextion display
9. *Plug in* display to flash the new design
10. *Unplug* display and remove the MicroSD card
11. *Plug in* the display again and expect to see your new design

## Instruction Set
The Nextion instruction set is a series of simple ASCII commands used to control the display and its components. Commands are sent from the host MCU to the Nextion display, and the display can send event data back. Each command is terminated by a special three-byte sequence: `0xFF 0xFF 0xFF`.
The function *void nextion_send_command*(*const char * *command*)* defined in [nextion.c](src/nextion.c) handles the sneding of nextion instructions to the display.

### Common Commands
- `page x`: Switches the display to page x (e.g., `page 1`).

- `t0.txt="Hello World"`: Sets the text property of a text component named t0 to "Hello World". This is used to display dynamic information.

- `b0.val=1`: Sets the value property of a component, like a progress bar b0, to 1.

- `va0.val=10`: Sets a variable component va0 to the value 10.

- `click b1,1`: Simulates a click on a button named b1.

- `dim=x`: Sets the display brightness level, where x is a value from 0 to 100.

## Electronic Characteristics
#### Power supply recommend：**5V, 1.0A, DC**
#### Operating Current: **220mA** (brightness 100%)

### Resources:
- [Datasheet](https://nextion.tech/datasheets/nx4827p043-011c/)
- [Instruction Set](https://nextion.tech/instruction-set/)
- [Nextion Editor Download](https://nextion.tech/nextion-editor/)



