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

#include "motors.h"
#include "TMC5272.h"
#include "mxc_delay.h"
#include "solenoid_driver.h"
#include <stdbool.h>

tmc_devices_t tmc_devices = {};
void init_motors(void) {
    // Create GPIO Port/Pins Config struct
    static mxc_gpio_cfg_t spi_port_cfg;
    spi_port_cfg = TMC5272_SPI_PORT_CFG_MXC;
    spi_port_cfg.vssel = MXC_GPIO_VSSEL_VDDIOH;

    // Add masks for X, Y, and TC axes.
    spi_port_cfg.mask |= (TMC5272_SPI_PIN_X | TMC5272_SPI_PIN_Y | TMC5272_SPI_PIN_TC);
    // Allocate static memory for device structs
    static tmc5272_dev_t tmc_x_dev = {
        .spi_port = MXC_SPI1,
        .gpio_cfg_spi = &spi_port_cfg,
        .ss_index = TMC5272_SPI_CS_IDX_X
    };
    
    static tmc5272_dev_t tmc_y_dev = {
        .spi_port = MXC_SPI1,
        .gpio_cfg_spi = &spi_port_cfg,
        .ss_index = TMC5272_SPI_CS_IDX_Y
    };
    
    static tmc5272_dev_t tmc_tc_dev = {
        .spi_port = MXC_SPI1,
        .gpio_cfg_spi = &spi_port_cfg,
        .ss_index = TMC5272_SPI_CS_IDX_TC
    };
    printf("Motors initialized");
    
    // Assign pointers to the tmc_devices
    tmc_devices.tmc_x = &tmc_x_dev;
    tmc_devices.tmc_y = &tmc_y_dev;
    tmc_devices.tmc_tc = &tmc_tc_dev;
    
    // Initialize independent motors
    tmc5272_init(tmc_devices.tmc_x);
    tmc5272_init(tmc_devices.tmc_y);

    /**** Motor Setup ****/
    // Init tricoders
    tmc5272_tricoder_init(tmc_devices.tmc_tc, TC_X);
    tmc5272_tricoder_init(tmc_devices.tmc_tc, TC_Y);
    
    // Velocity
    tmc5272_setVelocityCurve(tmc_devices.tmc_x, MOTOR_0, TMC_VEL_MAX, TMC_ACC_MAX);
    tmc5272_setVelocityCurve(tmc_devices.tmc_y, ALL_MOTORS, TMC_VEL_MAX, TMC_ACC_MAX);
    tmc5272_setSynchronizedPositioning(tmc_devices.tmc_y, true);
    
    tmc5272_setMotorPolarity(tmc_devices.tmc_x, MOTOR_0, MOTOR_DIR_INVERT);
    init_stallguard();
    // disable stallguard after homing sequence
    tmc5272_sg_setStallGuard2(tmc_devices.tmc_x, MOTOR_0, false);
    tmc5272_sg_setStallGuard2(tmc_devices.tmc_y, ALL_MOTORS, false);
    
}

tmc5272_dev_t* get_tmc_x_device(void) {
    // Safety check to ensure init_motors() was called
    if (tmc_devices.tmc_x == NULL) {
        return NULL;
    }
    return tmc_devices.tmc_x;
}

tmc5272_dev_t* get_tmc_y_device(void) {
    if (tmc_devices.tmc_y == NULL) {
        return NULL;
    }
    return tmc_devices.tmc_y;
}

tmc5272_dev_t* get_tmc_tc_device(void) {
    if (tmc_devices.tmc_tc == NULL) {
        return NULL;
    }
    return tmc_devices.tmc_tc;
}

// StallGuard Homing
void init_stallguard(void) {
    solenoid_on();
    // Y Axis
	tmc5272_sg_configureStallGuard2(tmc_devices.tmc_y, ALL_MOTORS, -20, 60, TRUE);
	tmc5272_sg_setStallGuard2(tmc_devices.tmc_y, ALL_MOTORS, TRUE);
	printf("Backing out y from edge... \n");
    
    MXC_Delay(MXC_DELAY_MSEC(500));
	tmc5272_rotateByMicrosteps(tmc_devices.tmc_y, ALL_MOTORS, 100000, 100000, 3000);
	while(!tmc5272_isAtTargetPosition(tmc_devices.tmc_y, MOTOR_0)) {
        printf("SGV: %d \n", tmc5272_sg_getSGValue(tmc_devices.tmc_x, MOTOR_0));
    }
	// Home to top: Rotate until stall
	tmc5272_rotateAtVelocity(tmc_devices.tmc_y, ALL_MOTORS, -1 * TMC_VEL_MAX, 4000);
	while(!tmc5272_sg_isStalled(tmc_devices.tmc_y, ALL_MOTORS)) {
        printf("SGV: %d \n", tmc5272_sg_getSGValue(tmc_devices.tmc_y, MOTOR_0));
    }
	// Stop movement before stall clear
	tmc5272_rotateAtVelocity(tmc_devices.tmc_y, ALL_MOTORS, 0, TMC_ACC_MAX);
	tmc5272_sg_clearStall(tmc_devices.tmc_y, ALL_MOTORS);
	// Set home position for y
	tmc5272_setPositionValue(tmc_devices.tmc_y, ALL_MOTORS, 0);
    // X Axis
    tmc5272_sg_configureStallGuard2(tmc_devices.tmc_x, ALL_MOTORS, 1, 60, TRUE);
    tmc5272_sg_setStallGuard2(tmc_devices.tmc_x, ALL_MOTORS, TRUE);
    printf("Backing out from edge... \n");
    MXC_Delay(MXC_DELAY_MSEC(500));
    tmc5272_rotateByMicrosteps(tmc_devices.tmc_x, MOTOR_0, 90000, 100000, 3000);
    while(!tmc5272_isAtTargetPosition(tmc_devices.tmc_x, MOTOR_0)) {}
    
    // Home to left: Rotate until stall
    tmc5272_rotateAtVelocity(tmc_devices.tmc_x, MOTOR_0, -300000, 4000);
    while(!tmc5272_sg_isStalled(tmc_devices.tmc_x, MOTOR_0)) {
    }
    // Stop movement before stall clear
    tmc5272_rotateAtVelocity(tmc_devices.tmc_x, MOTOR_0, 0, TMC_ACC_MAX);
    tmc5272_sg_clearStall(tmc_devices.tmc_x, MOTOR_0);
    // Set home position for x
    tmc5272_setPositionValue(tmc_devices.tmc_x, MOTOR_0, 0);
    tmc5272_sg_setStallGuard2(tmc_devices.tmc_x, MOTOR_0, false);
    tmc5272_sg_setStallGuard2(tmc_devices.tmc_y, ALL_MOTORS, false);
    solenoid_off();
    // End Stallguard homing
}