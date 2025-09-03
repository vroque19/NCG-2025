#ifndef SOLENOID_DRIVER_H
#define SOLENOID_DRIVER_H
// #include <stdio.h>
// #include <string.h>
#include "mxc_device.h"
// #include <stdint.h>
#include "board.h"
#include "gpio.h"

#define SOLENOID_GPIO_PORT_OUT   MXC_GPIO2
#define SOLENOID_GPIO_PIN_OUT    MXC_GPIO_PIN_13 // pin 15 does not work

void solenoid_gpio_init(void);
void solenoid_on(void);
void solenoid_off(void);
void solenoid_toggle(void);
#endif