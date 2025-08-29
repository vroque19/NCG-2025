/**
 * @file    main.c
 * @brief   Hello World!
 * @details This example uses the UART to print to a terminal and flashes an LED.
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
#define TC_X_In		MOTOR_0
#define TC_Y_In		MOTOR_1

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
	tmc5272_dev_t* motor_x = &(tmc5272_dev_t){
		.spi_port = MXC_SPI1,
		.gpio_cfg_spi = &spi_port_cfg,
		.ss_index = 1
	};
	tmc5272_dev_t* motor_y = &(tmc5272_dev_t){
		.spi_port = MXC_SPI1,
		.gpio_cfg_spi = &spi_port_cfg,
		.ss_index = 2
	};
	tmc5272_dev_t* motor_tc = &(tmc5272_dev_t){
		.spi_port = MXC_SPI1,
		.gpio_cfg_spi = &spi_port_cfg,
		.ss_index = 3
	};

	// Initialize each moving motor
	tmc5272_init(motor_x);
	tmc5272_init(motor_y);

	// Init tricoders
	tmc5272_tricoder_init(motor_tc, TC_X_In);
	tmc5272_tricoder_init(motor_tc, TC_Y_In);

	
	/**** Motor Setup ****/

	// Velocity
	tmc5272_setVelocityCurve(motor_x, MOTOR_0, 100000, 1000);
	tmc5272_setVelocityCurve(motor_y, MOTOR_0, 100000, 1000);


	/**** Main Program ****/

	// Start by rotating each motor
	tmc5272_rotateByMicrosteps(motor_x, MOTOR_0, 300000);
	MXC_Delay(MXC_DELAY_SEC(1));
	tmc5272_rotateByMicrosteps(motor_y, MOTOR_0, 200000);
	tmc5272_rotateByMicrosteps(motor_y, MOTOR_1, 200000);

	printf("Press PB to start Tricoder operation. \n");
	while(!PB_IsPressedAny()) {}
	MXC_Delay(MXC_DELAY_MSEC(500));

	// Main Loop
    while (1) {
	
		// Read the Tricoder position
		int32_t tc_x_pos = tmc5272_tricoder_getPosition(motor_tc, TC_X_In);
		int32_t tc_y_pos = tmc5272_tricoder_getPosition(motor_tc, TC_Y_In);

		// Rotate each axis to its encoder position
		tmc5272_rotateToPosition(motor_x, MOTOR_0, tc_x_pos);
		tmc5272_rotateToPosition(motor_y, MOTOR_0, 20*tc_y_pos);
		tmc5272_rotateToPosition(motor_y, MOTOR_1, 20*tc_y_pos);

		// Readout position & encoder
		printf("Current position: %d  ENC Position: %d \n", tmc5272_getPosition(motor_x,MOTOR_0), tc_x_pos);
		printf("Current position: %d  ENC Position: %d \n", tmc5272_getPosition(motor_y,MOTOR_0), tc_y_pos);

		// Failsafe brake
		if(PB_IsPressedAny())
		{
			tmc5272_rotateAtVelocity(motor_x, MOTOR_0, 0, 50000);
			tmc5272_rotateAtVelocity(motor_x, MOTOR_1, 0, 50000);
			tmc5272_rotateAtVelocity(motor_y, MOTOR_0, 0, 50000);
			tmc5272_rotateAtVelocity(motor_y, MOTOR_1, 0, 50000);
			
			while(1) {}
		}
		
		while(1) {}

    }
}
