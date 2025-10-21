/* 
MIT License

Copyright (c) 2025 Analog Devices

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef SOLENOID_DRIVER_H
#define SOLENOID_DRIVER_H
// #include <stdio.h>
// #include <string.h>
#include "mxc_device.h"
// #include <stdint.h>
#include "board.h"
#include "gpio.h"

#define SOLENOID_GPIO_PORT_OUT   MXC_GPIO2
#define SOLENOID_GPIO_PIN_OUT    MXC_GPIO_PIN_15

/**
* @brief initializes gpio pin for the solenoid driver
* @return void
 */
void solenoid_gpio_init(void);
void solenoid_on(void);
void solenoid_off(void);
void solenoid_toggle(void);
#endif