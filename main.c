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

	MXC_Delay(MXC_DELAY_MSEC(1500));


	// Startup Status Readout
	printf("\n\n\n");
    uint8_t spi_status = 0x00;

    uint32_t GCONF = tmc5272_readRegister(0, TMC5272_GCONF, &spi_status);
    printf("Status: 0x%X \n", spi_status);
    printf("GCONF: 0x%X \n\n", GCONF);

    uint32_t GSTAT = tmc5272_readRegister(0, TMC5272_GSTAT, &spi_status);
    printf("Status: 0x%X \n", spi_status);
    printf("GSTAT: 0x%X \n\n", GSTAT);

    // Rotate Motor
    printf("Rotating motor 0... \n");

	// Get to first rotation velocity
	printf("Rotating one way...\n");
	tmc5272_rotateAtVelocity(0, 0, 100000, ACC);
	while(!tmc5272_isAtTargetVelocity(0,0)) {
		printf("tmc5272 velocity: %d \n", tmc5272_getVelocity(0, 0));
	}

	// Slow to a stop
	printf("Slowing to stop...\n");
	tmc5272_rotateAtVelocity(0, 0, 0, ACC);
	while(!tmc5272_isAtTargetVelocity(0,0)) {
		printf("tmc5272 velocity: %d \n", tmc5272_getVelocity(0, 0));
	}

	// WAIT
	printf("PB to continue \n\n");
	while(!PB_IsPressedAny()) {}

	// Rotate the other way
	printf("Rotating other way...\n");
	tmc5272_rotateAtVelocity(0, 0, -100000, ACC);
	while(!tmc5272_isAtTargetVelocity(0,0)) {
		printf("tmc5272 velocity: %d \n", tmc5272_getVelocity(0, 0));
	}

	// Slow to a stop
	tmc5272_rotateAtVelocity(0, 0, 0, ACC*2);
	while(!tmc5272_isAtTargetVelocity(0,0)) {
		printf("tmc5272 velocity: %d \n", tmc5272_getVelocity(0, 0));
	}
    
	// WAIT
	printf("PB to continue \n\n");
	while(!PB_IsPressedAny()) {}


	// Set home position as current position + 1000 usteps
	uint32_t position = tmc5272_getPosition(0, 0);
	printf("Current Position: %d \n", position);
	tmc5272_setHomePosition(0, 0, position + 1000);
	position = tmc5272_getPosition(0, 0);
	printf("Now reads as %d \n", position);

	// WAIT
	printf("PB to continue \n\n");
	while(!PB_IsPressedAny()) {}

	// Positioning Tests
	// Set velocity curve and rotate fixed amounts
	printf("Set velocity curve: V=300k, A=10k \n");
	tmc5272_setVelocityCurve(0, 0, 300000, 10000);

	// WAIT
	printf("PB to continue \n\n");
	while(!PB_IsPressedAny()) {}

	// A fixed ustep rotation first
	printf("Full 2 rotations... \n");
	tmc5272_rotateByMicrosteps(0, 0, 204800); // Full rotation
	while(!tmc5272_isAtTargetPosition(0, 0)) {
		printf("(X,V): %d, %d \n", tmc5272_getPosition(0, 0), tmc5272_getVelocity(0, 0));
	}

	// WAIT
	printf("PB to continue \n\n");
	while(!PB_IsPressedAny()) {}

	printf("Restore to home position... \n");
	tmc5272_setVelocityCurve(0, 0, 300000, 1000);
	tmc5272_rotateToPosition(0, 0, 0);
	while(!tmc5272_isAtTargetPosition(0, 0)) {
		printf("(X,V): %d, %d \n", tmc5272_getPosition(0, 0), tmc5272_getVelocity(0, 0));
	}

	MXC_Delay(MXC_DELAY_MSEC(3000));

	uint32_t delay = 0;
    while (1) {
		if(delay++ > 1000000) {
			uint32_t rx = tmc5272_readRegister(0, TMC5272_RAMP_STAT(0), &spi_status);
			printf("M0_RAMP_STAT: 0x%X \n", rx);
			
			rx = tmc5272_getPosition(0, 0);
			printf("XACTUAL: %d \n", rx);

			rx = tmc5272_getVelocity(0, 0);
			printf("VACTUAL: %d \n", rx);

			printf("Status: 0x%X \n\n", spi_status);

			delay = 0;
		}

		// Red LED + rotate motor left/right when PB 0 or 1 pressed
		if(PB_Get(0) == TRUE) {
			tmc5272_rotateAtVelocity(0, 0, 100000, ACC);
			LED_On(0);
			while(PB_IsPressedAny()) {};

			tmc5272_rotateAtVelocity(0, 0, 0, ACC);
			LED_Off(0);
		}
		if(PB_Get(1) == TRUE) {
			tmc5272_rotateAtVelocity(0, 0, -100000, ACC);
			LED_On(0);
			while(PB_IsPressedAny()) {};

			tmc5272_rotateAtVelocity(0, 0, 0, ACC);
			LED_Off(0);
		}
		else {
			// do nothing
		}
    }
}
