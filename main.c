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
#include "TMC5272_SPI.h"

/* **** Definitions **** */
#define ACC 500
/* **** Globals **** */

/* **** Functions **** */

/* ************************************************************************** */
int main(void)
{
	// Initialize TMC Motor
	tmc5272_init(0);
	tmc5272_configEmergencyStop(0, 0, 1);
	tmc5272_configEmergencyStop(0, 1, 1);
	tmc5272_setVelocityCurve(0, 0, 50000, 10000);

	// Startup Status Readout
	printf("Tricoder Demo \n");
	printf("In this mode, Motor 0 rotates to match Motor 1. \n");

	// Tricoder Initialization
	tmc5272_tricoder_init(0, 1, tmc5272_getPosition(0, 0));
	tmc5272_tricoder_setBEMFHysteresis(0, 1, BEMF_HYST_10mV );

	// Main Loop
    while (1) {
	
		// Read the Tricoder position
		int32_t tricoder_position = tmc5272_tricoder_getPosition(0, 1);

		// Rotate motor 0 to TC position
		tmc5272_rotateToPosition(0, 0, tricoder_position);
		
		// Readout for register comparison
		if(PB_IsPressedAny())
		{
			for(uint8_t reg = 0x00; reg < 0x94; reg++)
			{
				uint32_t reg_val = tmc5272_readRegister(0, reg, NULL);
				printf("0x%X : 0x%X \n", reg, reg_val);
			}
		}
		// Readout
		// printf("Current position: %d \n \
		// 	ENC Position: %d \n", 
		// 	tmc5272_getPosition(0,0), tricoder_position);

    }
}
