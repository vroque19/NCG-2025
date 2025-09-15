/**
 * @file    main.c
 * @brief   Application code for Towers of Hanoi project
 * @details This is example code for the Trinamic motor control driver library 
 * used in Towers of Hanoi.
 */

/******************************************************************************
 *
 * Copyright (C) 2022-2023 Maxim Integrated Products, Inc. (now owned by 
 * Analog Devices, Inc.),
 * Copyright (C) 2023-2024 Analog Devices, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

/* **** Includes **** */
#include <stdio.h>
#include <stdint.h>
#include "led.h"
#include "pb.h"
#include "mxc_delay.h"

// TMC5272
#include "TMC5272.h"

/* **** Definitions **** */

// Map tricoder inputs for X and Y axes
#define TC_X		MOTOR_0
#define TC_Y		MOTOR_1

// TMC5272 Comms
#define TMC5272_SPI_PORT			MXC_SPI1
#define TMC5272_SPI_PORT_CFG_MXC	gpio_cfg_spi1

// SS Pin & Index
#define TMC5272_SPI_SS_PIN_DEV_X	MXC_GPIO_PIN_25
#define TMC5272_SPI_SS_IDX_DEV_X	1
#define TMC5272_SPI_SS_PIN_DEV_Y	MXC_GPIO_PIN_24
#define TMC5272_SPI_SS_IDX_DEV_Y	2
#define TMC5272_SPI_SS_PIN_DEV_TC	MXC_GPIO_PIN_27
#define TMC5272_SPI_SS_IDX_DEV_TC	3

// Velocity & Acceleration
#define VEL_X	300000
#define ACC_X	10000

#define VEL_Y	300000
#define ACC_Y	10000

/* **** Globals **** */

/* **** Functions **** */

/* ************************************************************************** */
int main(void)
{
	/**** Initialize TMC5272 Devices ******/

	// Create GPIO Port/Pins Config struct
	// Copy base MXC cfg struct. (MSDK uses const.)
    mxc_gpio_cfg_t spi_port_cfg = TMC5272_SPI_PORT_CFG_MXC;
	// Modify VSSEL (VDDIOH = 3.3V)
	spi_port_cfg.vssel = MXC_GPIO_VSSEL_VDDIOH;
	// Add masks for X, Y, and TC axes.
    spi_port_cfg.mask |= (TMC5272_SPI_SS_PIN_DEV_X | TMC5272_SPI_SS_PIN_DEV_Y | TMC5272_SPI_SS_PIN_DEV_TC);
	
	// Create device struct for each IC
	tmc5272_dev_t* tmc_x = &(tmc5272_dev_t){
		.spi_port = MXC_SPI1,
		.gpio_cfg_spi = &spi_port_cfg,
		.ss_index = 1
	};
	tmc5272_dev_t* tmc_y = &(tmc5272_dev_t){
		.spi_port = MXC_SPI1,
		.gpio_cfg_spi = &spi_port_cfg,
		.ss_index = 2
	};
	tmc5272_dev_t* tmc_tc = &(tmc5272_dev_t){
		.spi_port = MXC_SPI1,
		.gpio_cfg_spi = &spi_port_cfg,
		.ss_index = 3
	};

	// Initialize each moving motor
	printf("Init\n");
	tmc5272_init(tmc_x);
	tmc5272_init(tmc_y);

	// Init tricoders
	tmc5272_tricoder_init(tmc_tc, TC_X);
	tmc5272_tricoder_init(tmc_tc, TC_Y);

	// Failsafe setup
	tmc5272_rotateAtVelocity(tmc_x, ALL_MOTORS, 0, 8000);

	// Register dump & pause
	tmc5272_dumpRegisters(tmc_x);
	printf("Program halted. Press any pushbutton to continue... \n");
	while(!PB_IsPressedAny()) 
	{
		// Do nothing.
	}
	MXC_Delay(MXC_DELAY_MSEC(500));
	

	/**** Motor Setup ****/
	
	// Invert direction of necessary motors
	// tmc5272_setMotorDirection(tmc_y, MOTOR_0, MOTOR_DIR_INVERT);

	// StallGuard
	uint32_t SGT = 6;
	uint32_t TCOOLTHRS = 60;
	bool isFiltered = 1;
	tmc5272_configureStallGuard2(tmc_x, MOTOR_0, SGT, TCOOLTHRS, isFiltered);
	tmc5272_setStallGuard2(tmc_x, MOTOR_0, TRUE);


	/**** Main Program ****/

	// StallGuard Homing
	tmc5272_rotateAtVelocity(tmc_x, MOTOR_0, 300000, 2000);
	while(!tmc5272_sg_isStalled(tmc_x, MOTOR_0)) {
		
		printf("SGV: %d \n", tmc5272_sg_getSGValue(tmc_x, MOTOR_0));
		
	}
	LED_On(LED_RED);

	MXC_Delay(MXC_DELAY_SEC(2));
	tmc5272_sg_clearStall(tmc_x, MOTOR_0);
	LED_On(LED_GREEN);

	// Main Loop
	tmc5272_setHomePosition(tmc_x, ALL_MOTORS);

    while (1) {
		// Read the Tricoder position
		int32_t tc_x_pos = tmc5272_tricoder_getPosition(tmc_tc, TC_X);
		int32_t tc_y_pos = tmc5272_tricoder_getPosition(tmc_tc, TC_Y);

		// Rotate each axis to its encoder position
		tmc5272_rotateToPosition(tmc_x, MOTOR_0, 10*tc_x_pos, VEL_X, ACC_X);
		tmc5272_rotateToPosition(tmc_y, ALL_MOTORS, 10*tc_y_pos, VEL_Y, ACC_Y);

		// Readout position & encoder
		printf("Mx0: %d  ENC: %d", tmc5272_getPosition(tmc_x, MOTOR_0), tc_x_pos);
		printf("\tMy0: %d  ENC: %d \n", tmc5272_getPosition(tmc_y, MOTOR_0), tc_y_pos);

		// Failsafe brake
		if(PB_IsPressedAny())
		{
			printf("Failsafe! \n");
			tmc5272_rotateAtVelocity(tmc_x, MOTOR_0, 0, 50000);
			tmc5272_rotateAtVelocity(tmc_x, MOTOR_1, 0, 50000);
			tmc5272_rotateAtVelocity(tmc_y, MOTOR_0, 0, 50000);
			tmc5272_rotateAtVelocity(tmc_y, MOTOR_1, 0, 50000);
			
			while(1) {}
		}
		

    }
}
