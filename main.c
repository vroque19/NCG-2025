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
#define ACC 500
/* **** Globals **** */

/* **** Functions **** */

/* ************************************************************************** */
int main(void)
{
	// Initialize Motors
	tmc5272_dev_t motor_x = {
		.spi_port = MXC_SPI1,
		.gpio_cfg_spi_port = &gpio_cfg_spi1,
		.gpio_cfg_spi_ss = &gpio_cfg_spi1_ss1,
		.ss_pin_index = 1
	};
	tmc5272_init(&motor_x);

	tmc5272_dev_t motor_y = {
		.spi_port = MXC_SPI1,
		.gpio_cfg_spi_port = &gpio_cfg_spi1,
		.gpio_cfg_spi_ss = &gpio_cfg_spi1_ss2,
		.ss_pin_index = 2
	};
	tmc5272_init(&motor_y);	

	//tmc5272_configEmergencyStop(0, MOTOR_0, 1);
	//tmc5272_configEmergencyStop(0, MOTOR_1, 1);
	tmc5272_setVelocityCurve(&motor_x, MOTOR_0, 100000, 1000);
	tmc5272_setVelocityCurve(&motor_y, MOTOR_0, 100000, 1000);

	

	// Main Loop
    while (1) {
	
		tmc5272_rotateByMicrosteps(&motor_x, MOTOR_0, 300000);
		MXC_Delay(MXC_DELAY_SEC(5));
		tmc5272_rotateByMicrosteps(&motor_y, MOTOR_0, 200000);

		// Failsafe brake
		if(PB_IsPressedAny())
		{
			tmc5272_rotateAtVelocity(&motor_x, MOTOR_0, 0, 50000);
			tmc5272_rotateAtVelocity(&motor_y, MOTOR_0, 0, 50000);
			
			while(1) {}
		}
		
		// Do nothing
		while(1) {}

    }
}
