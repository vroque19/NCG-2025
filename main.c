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

/* **** Globals **** */

/* **** Functions **** */

/* ************************************************************************** */
int main(void)
{
    printf("\n\n\n");
    uint8_t spi_status = 0x00;

    uint32_t GCONF = tmc5272_readRegister(0, TMC5272_GCONF, &spi_status);
    printf("Status: 0x%X \n", spi_status);
    printf("GCONF: 0x%X \n\n", GCONF);

    uint32_t GSTAT = tmc5272_readRegister(0, TMC5272_GSTAT, &spi_status);
    printf("Status: 0x%X \n", spi_status);
    printf("GSTAT: 0x%X \n\n", GSTAT);

	//====================================================================================================//
	// ACTUAL SETTINGS FOR TMC5272 (created: 2025/07/31 16:55:09)                                        //
	//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv//

	tmc5272_writeRegister(0, 0x00, 	0x00020002, NULL); 		// writing value 0x00020002 = 131074 = 0.0 to address 0 = 0x00(GCONF)
	tmc5272_writeRegister(0, 0x03, 	0x00000300, NULL); 		// writing value 0x00000300 = 768 = 0.0 to address 1 = 0x03(SLAVECONF)
	tmc5272_writeRegister(0, 0x04, 	0x60223B19, NULL); 		// writing value 0x60223B19 = 1612856089 = 0.0 to address 2 = 0x04(IOIN)
	tmc5272_writeRegister(0, 0x05, 	0x0000000F, NULL); 		// writing value 0x0000000F = 15 = 0.0 to address 3 = 0x05(DRV_CONF)
	tmc5272_writeRegister(0, 0x06, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 4 = 0x06(GLOBAL_SCALER)
	tmc5272_writeRegister(0, 0x07, 	0x00000001, NULL); 		// writing value 0x00000001 = 1 = 0.0 to address 5 = 0x07(RAMPMODE)
	tmc5272_writeRegister(0, 0x08, 	0x00000009, NULL); 		// writing value 0x00000009 = 9 = 0.0 to address 6 = 0x08(MSLUT_ADDR)
	tmc5272_writeRegister(0, 0x09, 	0xFFFF8056, NULL); 		// writing value 0xFFFF8056 = 0 = 0.0 to address 7 = 0x09(MSLUT_DATA)
	tmc5272_writeRegister(0, 0x10, 	0xFFFFFFFF, NULL); 		// writing value 0xFFFFFFFF = 0 = 0.0 to address 8 = 0x10(M0_X_COMPARE)
	tmc5272_writeRegister(0, 0x11, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 9 = 0x11(M0_X_COMPARE_REPEAT)
	tmc5272_writeRegister(0, 0x12, 	0x04041F08, NULL); 		// writing value 0x04041F08 = 67378952 = 0.0 to address 10 = 0x12(M0_IHOLD_IRUN)
	tmc5272_writeRegister(0, 0x13, 	0x0000002B, NULL); 		// writing value 0x0000002B = 43 = 0.0 to address 11 = 0x13(M0_TPOWERDOWN)
	tmc5272_writeRegister(0, 0x15, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 12 = 0x15(M0_TPWMTHRS)
	tmc5272_writeRegister(0, 0x16, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 13 = 0x16(M0_TCOOLTHRS)
	tmc5272_writeRegister(0, 0x17, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 14 = 0x17(M0_THIGH)
	tmc5272_writeRegister(0, 0x18, 	0x026CA912, NULL); 		// writing value 0x026CA912 = 40675602 = 0.0 to address 15 = 0x18(M0_XACTUAL)
	tmc5272_writeRegister(0, 0x1B, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 16 = 0x1B(M0_VSTART)
	tmc5272_writeRegister(0, 0x1C, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 17 = 0x1C(M0_A1)
	tmc5272_writeRegister(0, 0x1D, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 18 = 0x1D(M0_V1)
	tmc5272_writeRegister(0, 0x1E, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 19 = 0x1E(M0_A2)
	tmc5272_writeRegister(0, 0x1F, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 20 = 0x1F(M0_V2)
	tmc5272_writeRegister(0, 0x20, 	0x00000F8D, NULL); 		// writing value 0x00000F8D = 3981 = 0.0 to address 21 = 0x20(M0_AMAX)
	tmc5272_writeRegister(0, 0x21, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 22 = 0x21(M0_VMAX)
	tmc5272_writeRegister(0, 0x22, 	0x00000F8D, NULL); 		// writing value 0x00000F8D = 3981 = 0.0 to address 23 = 0x22(M0_DMAX)
	tmc5272_writeRegister(0, 0x23, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 24 = 0x23(M0_D2)
	tmc5272_writeRegister(0, 0x24, 	0x0000000A, NULL); 		// writing value 0x0000000A = 10 = 0.0 to address 25 = 0x24(M0_D1)
	tmc5272_writeRegister(0, 0x25, 	0x0000000A, NULL); 		// writing value 0x0000000A = 10 = 0.0 to address 26 = 0x25(M0_VSTOP)
	tmc5272_writeRegister(0, 0x26, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 27 = 0x26(M0_TVMAX)
	tmc5272_writeRegister(0, 0x27, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 28 = 0x27(M0_TZEROWAIT)
	tmc5272_writeRegister(0, 0x28, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 29 = 0x28(M0_XTARGET)
	tmc5272_writeRegister(0, 0x29, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 30 = 0x29(M0_VDCMIN)
	tmc5272_writeRegister(0, 0x2A, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 31 = 0x2A(M0_SW_MODE)
	tmc5272_writeRegister(0, 0x2D, 	0x10000000, NULL); 		// writing value 0x10000000 = 268435456 = 0.0 to address 32 = 0x2D(M0_POSITION_PI_CTRL)
	tmc5272_writeRegister(0, 0x2E, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 33 = 0x2E(M0_X_ENC)
	tmc5272_writeRegister(0, 0x2F, 	0x00108000, NULL); 		// writing value 0x00108000 = 1081344 = 0.0 to address 34 = 0x2F(M0_ENCMODE)
	tmc5272_writeRegister(0, 0x30, 	0x00010000, NULL); 		// writing value 0x00010000 = 65536 = 0.0 to address 35 = 0x30(M0_ENC_CONST)
	tmc5272_writeRegister(0, 0x33, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 36 = 0x33(M0_ENC_DEVIATION)
	tmc5272_writeRegister(0, 0x34, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 37 = 0x34(M0_VIRTUAL_STOP_L)
	tmc5272_writeRegister(0, 0x35, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 38 = 0x35(M0_VIRTUAL_STOP_R)
	tmc5272_writeRegister(0, 0x38, 	0x10410153, NULL); 		// writing value 0x10410153 = 272695635 = 0.0 to address 39 = 0x38(M0_CHOPCONF)
	tmc5272_writeRegister(0, 0x39, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 40 = 0x39(M0_COOLCONF)
	tmc5272_writeRegister(0, 0x3A, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 41 = 0x3A(M0_DCCTRL)
	tmc5272_writeRegister(0, 0x3C, 	0xC40C001D, NULL); 		// writing value 0xC40C001D = 0 = 0.0 to address 42 = 0x3C(M0_PWMCONF)
	tmc5272_writeRegister(0, 0x3F, 	0x00000200, NULL); 		// writing value 0x00000200 = 512 = 0.0 to address 43 = 0x3F(M0_SG4_CONF)

	//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^//

    // Rotate Motor
    printf("Rotating motor 0... \n");

    // Clear reset flag
    tmc5272_fieldWrite(0, TMC5272_GSTAT_REGISTER_RESET_FIELD, 1);
	
	// Reset XACTUAL position to 0 ("home")
	tmc5272_fieldWrite(0, TMC5272_XACTUAL_FIELD(0), 0);


	uint32_t delay = 0;
    while (1) {
		if(delay++ > 1000000) {
			uint32_t rx = tmc5272_readRegister(0, TMC5272_RAMP_STAT(0), &spi_status);
			printf("M0_RAMP_STAT: 0x%X \n", rx);
			
			rx = tmc5272_readRegister(0, TMC5272_XACTUAL(0), &spi_status);
			printf("XACTUAL: 0x%X \n", rx);

			rx = tmc5272_readRegister(0, TMC5272_VACTUAL(0), NULL);
			printf("VACTUAL: 0x%X \n", rx);


			printf("Status: 0x%X \n\n", spi_status);

			delay = 0;
		}

		// Red LED + rotate motor left/right when PB 0 or 1 pressed
		if(PB_Get(0) == TRUE) {
			tmc5272_rotateMotor(0, 0, 100000);
			LED_On(0);
			while(PB_IsPressedAny()) {};

			tmc5272_rotateMotor(0, 0, 0);
			LED_Off(0);
		}
		if(PB_Get(1) == TRUE) {
			tmc5272_rotateMotor(0, 0, -100000);
			LED_On(0);
			while(PB_IsPressedAny()) {};

			tmc5272_rotateMotor(0, 0, 0);
			LED_Off(0);
		}
		else {
			// do nothing
		}
    }
}
