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

#ifndef MOTORS_H
#define MOTORS_H

#include "TMC5272.h"
// /**** Macros ****/

// // Map tricoder inputs for X and Y axes
#define TC_X		MOTOR_0
#define TC_Y		MOTOR_1

// // TMC5272 Comms
#define TMC5272_SPI_PORT			MXC_SPI1
#define TMC5272_SPI_PORT_CFG_MXC	gpio_cfg_spi1

// // SS Pin & Index
#define TMC5272_SPI_PIN_X	MXC_GPIO_PIN_25
#define TMC5272_SPI_CS_IDX_X	1
#define TMC5272_SPI_PIN_Y	MXC_GPIO_PIN_24
#define TMC5272_SPI_CS_IDX_Y	2
#define TMC5272_SPI_PIN_TC	MXC_GPIO_PIN_27
#define TMC5272_SPI_CS_IDX_TC	3

// Velocity and Acceleration Curve
#define TMC_VEL_MAX 300000
#define TMC_ACC_MAX 10000


// Holds all 3 tmc devices
typedef struct {
    tmc5272_dev_t* tmc_x;
    tmc5272_dev_t* tmc_y;
    tmc5272_dev_t* tmc_tc;
} tmc_devices_t;
extern tmc_devices_t tmc_devices;
// Function to get the initialized device structs
tmc5272_dev_t* get_tmc_x_device(void);
tmc5272_dev_t* get_tmc_y_device(void);
tmc5272_dev_t* get_tmc_tc_device(void);
void init_motors(void);
void init_stallguard(void);

#endif