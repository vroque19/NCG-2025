/*******************************************************************************
* Copyright © 2019 TRINAMIC Motion Control GmbH & Co. KG
* (now owned by Analog Devices Inc.),
*
* Copyright © 2024 Analog Devices Inc. All Rights Reserved.
* This software is proprietary to Analog Devices, Inc. and its licensors.
*******************************************************************************/
#include "TMC5272.h"

/* Function Definitions */

int32_t tmc5272_readRegister(tmc5272_dev_t* tmc5272_dev, uint8_t address, uint8_t* spi_status)
{
	uint8_t tx_data[5] = { 0 };
	uint8_t rx_data[5] = { 0 };

	// clear write bit
	tx_data[0] = address & TMC5272_ADDRESS_MASK;

	// Send the read request
	tmc5272_SPI_readWrite(tmc5272_dev, &tx_data[0], sizeof(tx_data), rx_data);

	// Rewrite address and clear write bit
	tx_data[0] = address & TMC5272_ADDRESS_MASK;

	// Send another request to receive the read reply
	tmc5272_SPI_readWrite(tmc5272_dev, &tx_data[0], sizeof(tx_data), rx_data);

	// Return status and data
	*spi_status = rx_data[0];
	return ((int32_t)rx_data[1] << 24) | ((int32_t) rx_data[2] << 16) | ((int32_t) rx_data[3] <<  8) | ((int32_t) rx_data[4]);
}

void tmc5272_writeRegister(tmc5272_dev_t* tmc5272_dev, uint8_t address, int32_t value, uint8_t* spi_status)
{
	uint8_t tx_data[5] = { 0 };
	uint8_t rx_data[5] = { 0 };

	tx_data[0] = address | TMC5272_WRITE_BIT;
	tx_data[1] = 0xFF & (value>>24);
	tx_data[2] = 0xFF & (value>>16);
	tx_data[3] = 0xFF & (value>>8);
	tx_data[4] = 0xFF & (value>>0);

	// Send the write request
	tmc5272_SPI_readWrite(tmc5272_dev, &tx_data[0], sizeof(tx_data), rx_data);

	// Store status received in rx
	*spi_status = rx_data[0];
}

void tmc5272_init(tmc5272_dev_t* tmc5272_dev)
{
	/**** Comms Initialization ****/
	tmc5272_SPI_init(tmc5272_dev);

	/**** Device Register Initialization ****/
	// Recommendation: Get register settings from TMCL-IDE -> Export from Register Browser.

	// Set HOLD mode FIRST!
	tmc5272_writeRegister(tmc5272_dev, TMC5272_RAMPMODE, 0x0000000F, NULL);
	
	// IHOLD_IRUN
	// IRUN = 32/32; IHOLD = 8
	tmc5272_writeRegister(tmc5272_dev, TMC5272_IHOLD_IRUN(MOTOR_0), 0x04011F08, NULL); 
	tmc5272_writeRegister(tmc5272_dev, TMC5272_IHOLD_IRUN(MOTOR_1), 0x04011F08, NULL); 

	//====================================================================================================//
// ACTUAL SETTINGS FOR TMC5272 (created: 2025/09/03 16:20:11)                                        //
//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv//

	tmc5272_writeRegister(tmc5272_dev, 0x00, 	0x00020002, NULL); 		// writing value 0x00020002 = 131074 = 0.0 to address 0 = 0x00(GCONF)
	tmc5272_writeRegister(tmc5272_dev, 0x03, 	0x00000300, NULL); 		// writing value 0x00000300 = 768 = 0.0 to address 1 = 0x03(SLAVECONF)
	tmc5272_writeRegister(tmc5272_dev, 0x04, 	0x60223318, NULL); 		// writing value 0x60223318 = 1612854040 = 0.0 to address 2 = 0x04(IOIN)
	tmc5272_writeRegister(tmc5272_dev, 0x05, 	0x000003CF, NULL); 		// writing value 0x000003CF = 975 = 0.0 to address 3 = 0x05(DRV_CONF)
	tmc5272_writeRegister(tmc5272_dev, 0x06, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 4 = 0x06(GLOBAL_SCALER)
	tmc5272_writeRegister(tmc5272_dev, 0x08, 	0x00000019, NULL); 		// writing value 0x00000019 = 25 = 0.0 to address 6 = 0x08(MSLUT_ADDR)
	tmc5272_writeRegister(tmc5272_dev, 0x09, 	0xFFFF8056, NULL); 		// writing value 0xFFFF8056 = 0 = 0.0 to address 7 = 0x09(MSLUT_DATA)
	tmc5272_writeRegister(tmc5272_dev, 0x10, 	0xFFFFFFFF, NULL); 		// writing value 0xFFFFFFFF = 0 = 0.0 to address 8 = 0x10(M0_X_COMPARE)
	tmc5272_writeRegister(tmc5272_dev, 0x11, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 9 = 0x11(M0_X_COMPARE_REPEAT)
	tmc5272_writeRegister(tmc5272_dev, 0x13, 	0x0000000A, NULL); 		// writing value 0x0000000A = 10 = 0.0 to address 11 = 0x13(M0_TPOWERDOWN)
	tmc5272_writeRegister(tmc5272_dev, 0x15, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 12 = 0x15(M0_TPWMTHRS)
	tmc5272_writeRegister(tmc5272_dev, 0x16, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 13 = 0x16(M0_TCOOLTHRS)
	tmc5272_writeRegister(tmc5272_dev, 0x17, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 14 = 0x17(M0_THIGH)
	tmc5272_writeRegister(tmc5272_dev, 0x18, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 15 = 0x18(M0_XACTUAL)
	tmc5272_writeRegister(tmc5272_dev, 0x1B, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 16 = 0x1B(M0_VSTART)
	tmc5272_writeRegister(tmc5272_dev, 0x1C, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 17 = 0x1C(M0_A1)
	tmc5272_writeRegister(tmc5272_dev, 0x1D, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 18 = 0x1D(M0_V1)
	tmc5272_writeRegister(tmc5272_dev, 0x1E, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 19 = 0x1E(M0_A2)
	tmc5272_writeRegister(tmc5272_dev, 0x1F, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 20 = 0x1F(M0_V2)
	tmc5272_writeRegister(tmc5272_dev, 0x20, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 21 = 0x20(M0_AMAX)
	tmc5272_writeRegister(tmc5272_dev, 0x21, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 22 = 0x21(M0_VMAX)
	tmc5272_writeRegister(tmc5272_dev, 0x22, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 23 = 0x22(M0_DMAX)
	tmc5272_writeRegister(tmc5272_dev, 0x23, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 24 = 0x23(M0_D2)
	tmc5272_writeRegister(tmc5272_dev, 0x24, 	0x0000000A, NULL); 		// writing value 0x0000000A = 10 = 0.0 to address 25 = 0x24(M0_D1)
	tmc5272_writeRegister(tmc5272_dev, 0x25, 	0x0000000A, NULL); 		// writing value 0x0000000A = 10 = 0.0 to address 26 = 0x25(M0_VSTOP)
	tmc5272_writeRegister(tmc5272_dev, 0x26, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 27 = 0x26(M0_TVMAX)
	tmc5272_writeRegister(tmc5272_dev, 0x27, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 28 = 0x27(M0_TZEROWAIT)
	tmc5272_writeRegister(tmc5272_dev, 0x28, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 29 = 0x28(M0_XTARGET)
	tmc5272_writeRegister(tmc5272_dev, 0x29, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 30 = 0x29(M0_VDCMIN)
	tmc5272_writeRegister(tmc5272_dev, 0x2A, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 31 = 0x2A(M0_SW_MODE)
	tmc5272_writeRegister(tmc5272_dev, 0x2D, 	0x10000000, NULL); 		// writing value 0x10000000 = 268435456 = 0.0 to address 32 = 0x2D(M0_POSITION_PI_CTRL)
	tmc5272_writeRegister(tmc5272_dev, 0x2E, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 33 = 0x2E(M0_X_ENC)
	tmc5272_writeRegister(tmc5272_dev, 0x2F, 	0x00108000, NULL); 		// writing value 0x00108000 = 1081344 = 0.0 to address 34 = 0x2F(M0_ENCMODE)
	tmc5272_writeRegister(tmc5272_dev, 0x30, 	0x00010000, NULL); 		// writing value 0x00010000 = 65536 = 0.0 to address 35 = 0x30(M0_ENC_CONST)
	tmc5272_writeRegister(tmc5272_dev, 0x33, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 36 = 0x33(M0_ENC_DEVIATION)
	tmc5272_writeRegister(tmc5272_dev, 0x34, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 37 = 0x34(M0_VIRTUAL_STOP_L)
	tmc5272_writeRegister(tmc5272_dev, 0x35, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 38 = 0x35(M0_VIRTUAL_STOP_R)
	tmc5272_writeRegister(tmc5272_dev, 0x38, 	0x10410153, NULL); 		// writing value 0x10410153 = 272695635 = 0.0 to address 39 = 0x38(M0_CHOPCONF)
	tmc5272_writeRegister(tmc5272_dev, 0x39, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 40 = 0x39(M0_COOLCONF)
	tmc5272_writeRegister(tmc5272_dev, 0x3A, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 41 = 0x3A(M0_DCCTRL)
	tmc5272_writeRegister(tmc5272_dev, 0x3C, 	0xC40C001D, NULL); 		// writing value 0xC40C001D = 0 = 0.0 to address 42 = 0x3C(M0_PWMCONF)
	tmc5272_writeRegister(tmc5272_dev, 0x3F, 	0x00000200, NULL); 		// writing value 0x00000200 = 512 = 0.0 to address 43 = 0x3F(M0_SG4_CONF)
	tmc5272_writeRegister(tmc5272_dev, 0x45, 	0xFFFFFFFF, NULL); 		// writing value 0xFFFFFFFF = 0 = 0.0 to address 44 = 0x45(M1_X_COMPARE)
	tmc5272_writeRegister(tmc5272_dev, 0x46, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 45 = 0x46(M1_X_COMPARE_REPEAT)
	tmc5272_writeRegister(tmc5272_dev, 0x47, 	0x04011F08, NULL); 		// writing value 0x04011F08 = 67182344 = 0.0 to address 46 = 0x47(M1_IHOLD_IRUN)
	tmc5272_writeRegister(tmc5272_dev, 0x48, 	0x0000000A, NULL); 		// writing value 0x0000000A = 10 = 0.0 to address 47 = 0x48(M1_TPOWERDOWN)
	tmc5272_writeRegister(tmc5272_dev, 0x4A, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 48 = 0x4A(M1_TPWMTHRS)
	tmc5272_writeRegister(tmc5272_dev, 0x4B, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 49 = 0x4B(M1_TCOOLTHRS)
	tmc5272_writeRegister(tmc5272_dev, 0x4C, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 50 = 0x4C(M1_THIGH)
	tmc5272_writeRegister(tmc5272_dev, 0x4D, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 51 = 0x4D(M1_XACTUAL)
	tmc5272_writeRegister(tmc5272_dev, 0x50, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 52 = 0x50(M1_VSTART)
	tmc5272_writeRegister(tmc5272_dev, 0x51, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 53 = 0x51(M1_A1)
	tmc5272_writeRegister(tmc5272_dev, 0x52, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 54 = 0x52(M1_V1)
	tmc5272_writeRegister(tmc5272_dev, 0x53, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 55 = 0x53(M1_A2)
	tmc5272_writeRegister(tmc5272_dev, 0x54, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 56 = 0x54(M1_V2)
	tmc5272_writeRegister(tmc5272_dev, 0x55, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 57 = 0x55(M1_AMAX)
	tmc5272_writeRegister(tmc5272_dev, 0x56, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 58 = 0x56(M1_VMAX)
	tmc5272_writeRegister(tmc5272_dev, 0x57, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 59 = 0x57(M1_DMAX)
	tmc5272_writeRegister(tmc5272_dev, 0x58, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 60 = 0x58(M1_D2)
	tmc5272_writeRegister(tmc5272_dev, 0x59, 	0x0000000A, NULL); 		// writing value 0x0000000A = 10 = 0.0 to address 61 = 0x59(M1_D1)
	tmc5272_writeRegister(tmc5272_dev, 0x5A, 	0x0000000A, NULL); 		// writing value 0x0000000A = 10 = 0.0 to address 62 = 0x5A(M1_VSTOP)
	tmc5272_writeRegister(tmc5272_dev, 0x5B, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 63 = 0x5B(M1_TVMAX)
	tmc5272_writeRegister(tmc5272_dev, 0x5C, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 64 = 0x5C(M1_TZEROWAIT)
	tmc5272_writeRegister(tmc5272_dev, 0x5D, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 65 = 0x5D(M1_XTARGET)
	tmc5272_writeRegister(tmc5272_dev, 0x5E, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 66 = 0x5E(M1_VDCMIN)
	tmc5272_writeRegister(tmc5272_dev, 0x5F, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 67 = 0x5F(M1_SW_MODE)
	tmc5272_writeRegister(tmc5272_dev, 0x62, 	0x10000000, NULL); 		// writing value 0x10000000 = 268435456 = 0.0 to address 68 = 0x62(M1_POSITION_PI_CTRL)
	tmc5272_writeRegister(tmc5272_dev, 0x63, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 69 = 0x63(M1_X_ENC)
	tmc5272_writeRegister(tmc5272_dev, 0x64, 	0x00108000, NULL); 		// writing value 0x00108000 = 1081344 = 0.0 to address 70 = 0x64(M1_ENCMODE)
	tmc5272_writeRegister(tmc5272_dev, 0x65, 	0x00010000, NULL); 		// writing value 0x00010000 = 65536 = 0.0 to address 71 = 0x65(M1_ENC_CONST)
	tmc5272_writeRegister(tmc5272_dev, 0x68, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 72 = 0x68(M1_ENC_DEVIATION)
	tmc5272_writeRegister(tmc5272_dev, 0x69, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 73 = 0x69(M1_VIRTUAL_STOP_L)
	tmc5272_writeRegister(tmc5272_dev, 0x6A, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 74 = 0x6A(M1_VIRTUAL_STOP_R)
	tmc5272_writeRegister(tmc5272_dev, 0x6D, 	0x10410153, NULL); 		// writing value 0x10410153 = 272695635 = 0.0 to address 75 = 0x6D(M1_CHOPCONF)
	tmc5272_writeRegister(tmc5272_dev, 0x6E, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 76 = 0x6E(M1_COOLCONF)
	tmc5272_writeRegister(tmc5272_dev, 0x6F, 	0x00000000, NULL); 		// writing value 0x00000000 = 0 = 0.0 to address 77 = 0x6F(M1_DCCTRL)
	tmc5272_writeRegister(tmc5272_dev, 0x71, 	0xC40C001D, NULL); 		// writing value 0xC40C001D = 0 = 0.0 to address 78 = 0x71(M1_PWMCONF)
	tmc5272_writeRegister(tmc5272_dev, 0x74, 	0x00000200, NULL); 		// writing value 0x00000200 = 512 = 0.0 to address 79 = 0x74(M1_SG4_CONF)

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^//
	// Clear reset flag
    tmc5272_fieldWrite(tmc5272_dev, TMC5272_GSTAT_REGISTER_RESET_FIELD, 1);
	
	// Reset XACTUAL position to 0 ("home")
	tmc5272_fieldWrite(tmc5272_dev, TMC5272_XACTUAL_FIELD(MOTOR_0), 0);
	tmc5272_fieldWrite(tmc5272_dev, TMC5272_XACTUAL_FIELD(MOTOR_1), 0);

}

void tmc5272_configEmergencyStop(tmc5272_dev_t* tmc5272_dev, uint8_t motor, uint8_t isEnabled)
{
	if(motor == MOTOR_0)
	{
		tmc5272_fieldWrite(tmc5272_dev, TMC5272_GCONF_M0_STOP_ENABLE_FIELD, isEnabled);
	}
	else
	{
		tmc5272_fieldWrite(tmc5272_dev, TMC5272_GCONF_M1_STOP_ENABLE_FIELD, isEnabled);
	}
}

// Movement Commands
uint32_t tmc5272_getPosition(tmc5272_dev_t* tmc5272_dev, uint8_t motor) 
{
	return tmc5272_fieldRead(tmc5272_dev, TMC5272_XACTUAL_FIELD(motor));
}

int32_t tmc5272_getVelocity(tmc5272_dev_t* tmc5272_dev, uint8_t motor)
{
	return tmc5272_fieldRead(tmc5272_dev, TMC5272_VACTUAL_FIELD(motor));
}

bool tmc5272_isAtTargetVelocity(tmc5272_dev_t* tmc5272_dev, uint8_t motor)
{
	return tmc5272_fieldRead(tmc5272_dev, TMC5272_RAMP_STAT_VELOCITY_REACHED_FIELD(motor));
}

bool tmc5272_isAtTargetPosition(tmc5272_dev_t* tmc5272_dev, uint8_t motor)
{
	return (tmc5272_getPosition(tmc5272_dev, motor) == tmc5272_fieldRead(tmc5272_dev, TMC5272_XTARGET_FIELD(motor)));
}

void tmc5272_setHomePosition(tmc5272_dev_t* tmc5272_dev, uint8_t motor, uint32_t target_position)
{
	uint32_t new_position = tmc5272_getPosition(tmc5272_dev, motor) - target_position;
	tmc5272_fieldWrite(tmc5272_dev, TMC5272_XACTUAL_FIELD(motor), new_position);

	return;
}

void tmc5272_setVelocityCurve(tmc5272_dev_t* tmc5272_dev, uint8_t motor, uint32_t vmax, uint32_t amax) 
{
	// Set RAMPMODE = Hold to retain whatever (lack of) motion is occurring.
	tmc5272_fieldWrite(tmc5272_dev, TMC5272_RAMPMODE_FIELD(motor), TMC5272_MODE_HOLD);

	// Set V1 and V2 = 0.
	// This disables A1 and A2 phases of the velocity curve.
	tmc5272_fieldWrite(tmc5272_dev, TMC5272_V1_FIELD(motor), 0);
	tmc5272_fieldWrite(tmc5272_dev, TMC5272_V2_FIELD(motor), 0);

	// Set AMAX and DMAX.
	tmc5272_fieldWrite(tmc5272_dev, TMC5272_AMAX_FIELD(motor), amax);
	tmc5272_fieldWrite(tmc5272_dev, TMC5272_DMAX_FIELD(motor), amax);

	// Set velocity.
	tmc5272_fieldWrite(tmc5272_dev, TMC5272_VMAX_FIELD(motor), vmax);
}

void tmc5272_rotateAtVelocity(tmc5272_dev_t* tmc5272_dev, uint8_t motor, int32_t velocity, uint32_t acceleration)
{
    if(motor >= TMC5272_MOTORS)
		return;

	// Convert signed velocity to unsigned with direction
	uint8_t rampMode = TMC5272_MODE_VELPOS;
	if(velocity < 0) {
		velocity = -velocity;
		rampMode = TMC5272_MODE_VELNEG;
	}
	// Set velocity
	tmc5272_setVelocityCurve(tmc5272_dev, motor, (uint32_t)velocity, acceleration);
	
	// Modify RAMPMODE to use velocity control and set direction
	// Note: We modify rampmode _after_ setting the velocity curve, in
	// case velocity was an unreasonable value (e.g. 0xFFFFFFFF)!
	tmc5272_fieldWrite(tmc5272_dev, TMC5272_RAMPMODE_FIELD(motor), rampMode);

	return;
}

/* Note: Call position rotation functions after setting velocity curve.*/
void tmc5272_rotateToPosition(tmc5272_dev_t* tmc5272_dev, uint8_t motor, uint32_t target)
{
	// If not in Position mode, switch to HOLD mode to avoid changing anything.
	// (If we're already in position mode, just stay in position mode.)
	if(tmc5272_fieldRead(tmc5272_dev, TMC5272_RAMPMODE_FIELD(motor)) != TMC5272_MODE_POSITION) 
	{
		tmc5272_fieldWrite(tmc5272_dev, TMC5272_RAMPMODE_FIELD(motor), TMC5272_MODE_HOLD);
	}
	// Write target position
	tmc5272_fieldWrite(tmc5272_dev, TMC5272_XTARGET_FIELD(motor), target);

	// Switch to Position mode
	tmc5272_fieldWrite(tmc5272_dev, TMC5272_RAMPMODE_FIELD(motor), TMC5272_MODE_POSITION);
}

/* Note: Call position rotation functions after setting velocity curve.*/
void tmc5272_rotateByMicrosteps(tmc5272_dev_t* tmc5272_dev, uint8_t motor, int32_t usteps)
{
	// Get new target position
	uint32_t target_position = tmc5272_getPosition(tmc5272_dev, motor) + usteps;
	
	// Rotate to it
	tmc5272_rotateToPosition(tmc5272_dev, motor, target_position);
}


/* Tricoder */

// The initialization of Tricoder does not halt the motor. This is left to the application.
// Default BEMF hysteresis of 10mV.
// Default encoder value set to 0.
void tmc5272_tricoder_init(tmc5272_dev_t* tmc5272_dev, uint8_t motor)
{
	// From "Relevant Settings" in Tricoder section of datasheet

	// 0x1: StealthChop2 enabled
	if(motor == MOTOR_0) {
		tmc5272_fieldWrite(tmc5272_dev, TMC5272_GCONF_M0_EN_PWM_MODE_FIELD, 1);
	}
	if(motor == MOTOR_1) {
		tmc5272_fieldWrite(tmc5272_dev, TMC5272_GCONF_M1_EN_PWM_MODE_FIELD, 1);
	}

	// 0x0: Lowest standstill (hold) current at 1/32 full-scale. 
	// Must set to 0 to choose freewheel / coil short circuit for standstill.
	tmc5272_fieldWrite(tmc5272_dev, TMC5272_IHOLD_IRUN_IHOLD_FIELD(motor), 0);

	// 0x0: Disable external encoder inputs. Motor must be at standstill.
	tmc5272_fieldWrite(tmc5272_dev, TMC5272_ENCMODE_NBEMF_ABN_SEL_FIELD(motor), 0);
	// 0x2: Passive braking
	tmc5272_fieldWrite(tmc5272_dev, TMC5272_PWMCONF_FREEWHEEL_FIELD(motor), 2);

	// 0x0: TOFF off time. NCLK = 24 + 32 * TOFF. 0 = Driver disable, all bridges off.
	// tmc5272_fieldWrite(tmc5272_dev, TMC5272_CHOPCONF_TOFF_FIELD(motor), 0);
	// Note: The above is directed from the datasheet; however, TMCL-IDE sets the following:
	tmc5272_fieldWrite(tmc5272_dev, TMC5272_CHOPCONF_TOFF_FIELD(motor), 0x3);

	// 0x00108000: All defaults. This covers qsc_enc_en that datasheet instructs to set to 1.
	tmc5272_writeRegister(tmc5272_dev, TMC5272_ENCMODE(motor), 0x00108000, NULL); 

	// ENC_CONST: Set multiplying factor per encoder step.
	// Tricoder only detects fullsteps (200usteps), which is a factor of 256. 
	// Refer to Table 26 for encoder factor <--> encoder resolution conversion.
	tmc5272_fieldWrite(tmc5272_dev, TMC5272_ENC_CONST_FIELD(motor), 256<<16);

	// Set Mx_X_ENC: Set encoder position to 0.
	tmc5272_fieldWrite(tmc5272_dev, TMC5272_X_ENC_FIELD(motor), 0);

	// Set FSR_Mx: 0x3: KIFS = 8.00, typ R_DS(ON),LS = 0.11Ohms.
	// This is not in the TMC5272 datasheet Tricoder section.
	// However, this sets the current sense to increase the overcurrent protection threshold.
	// Leaving this at 0x0 causes a "short to supply" to be detected 
	// when the tricoder is spun quickly.
	// Therefore, increase it to prevent this error condition.
	if(motor == MOTOR_0)
	{
		tmc5272_fieldWrite(tmc5272_dev, TMC5272_DRV_CONF_FSR_M0_FIELD, 0x3);
	}
	if(motor == MOTOR_1)
	{
		tmc5272_fieldWrite(tmc5272_dev, TMC5272_DRV_CONF_FSR_M1_FIELD, 0x3);
	}

	// Reset from possible coil short condition -- this just cycles driver disable / enable to reset.
	tmc5272_tricoder_resetFromCoilShort(tmc5272_dev, motor);
}

// Detect supply-side coil short. This occurs when the tricoder motor is spun too quickly.
// 0 = No short.
// 1 = Short motor phase A
// 2 = Short motor phase B
// 3 = Short both phases (binary 11)
uint8_t tmc5272_tricoder_isCoilShortVS(tmc5272_dev_t* tmc5272_dev, uint8_t motor)
{
	// Read DRV_STATUS register
	uint32_t drv_status = tmc5272_readRegister(tmc5272_dev, TMC5272_DRV_STATUS(motor), NULL);

	// Coil short will be on phase A or B
	uint8_t is_coil_short = (drv_status & (TMC5272_DRV_STATUS_S2VSB_MASK | TMC5272_DRV_STATUS_S2VSA_MASK)) >> TMC5272_DRV_STATUS_S2VSA_SHIFT;

	return is_coil_short;
}

void tmc5272_tricoder_resetFromCoilShort(tmc5272_dev_t* tmc5272_dev, uint8_t motor)
{
	if(motor == 0)
	{
		// Toggle: Disable (1) and enable (0)
		tmc5272_fieldWrite(tmc5272_dev, TMC5272_GCONF_M0_DRV_ENN_FIELD, 1);
		tmc5272_fieldWrite(tmc5272_dev, TMC5272_GCONF_M0_DRV_ENN_FIELD, 0);
	}
	if(motor == 1)
	{
		// Toggle: Disable (1) and enable (0)
		tmc5272_fieldWrite(tmc5272_dev, TMC5272_GCONF_M1_DRV_ENN_FIELD, 1);
		tmc5272_fieldWrite(tmc5272_dev, TMC5272_GCONF_M1_DRV_ENN_FIELD, 0);
	}
}

void tmc5272_tricoder_setBEMFHysteresis(tmc5272_dev_t* tmc5272_dev, uint8_t motor, tmc5272_tricoder_bemf_hysteresis_t hysteresis)
{
	tmc5272_fieldWrite(tmc5272_dev, TMC5272_ENCMODE_BEMF_HYST_FIELD(motor), hysteresis);
}

void tmc5272_tricoder_setEncoderValue(tmc5272_dev_t* tmc5272_dev, uint8_t motor, int32_t encoder_value)
{
	tmc5272_fieldWrite(tmc5272_dev, TMC5272_X_ENC_FIELD(motor), encoder_value);
}

int32_t tmc5272_tricoder_getPosition(tmc5272_dev_t* tmc5272_dev, uint8_t motor)
{
	return (int32_t)tmc5272_fieldRead(tmc5272_dev, TMC5272_X_ENC_FIELD(motor));
}