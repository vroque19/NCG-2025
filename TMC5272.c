/*******************************************************************************
* Copyright © 2019 TRINAMIC Motion Control GmbH & Co. KG
* (now owned by Analog Devices Inc.),
*
* Copyright © 2024 Analog Devices Inc. All Rights Reserved.
* This software is proprietary to Analog Devices, Inc. and its licensors.
*******************************************************************************/
#include "TMC5272.h"


/* Function Definitions */

int32_t tmc5272_readRegister(uint16_t icID, uint8_t address, uint8_t* spi_status)
{
	uint8_t tx_data[5] = { 0 };
	uint8_t rx_data[5] = { 0 };

	// clear write bit
	tx_data[0] = address & TMC5272_ADDRESS_MASK;

	// Send the read request
	tmc5272_readWriteSPI(icID, &tx_data[0], sizeof(tx_data), rx_data);

	// Rewrite address and clear write bit
	tx_data[0] = address & TMC5272_ADDRESS_MASK;

	// Send another request to receive the read reply
	tmc5272_readWriteSPI(icID, &tx_data[0], sizeof(tx_data), rx_data);

	// Return status and data
	*spi_status = rx_data[0];
	return ((int32_t)rx_data[1] << 24) | ((int32_t) rx_data[2] << 16) | ((int32_t) rx_data[3] <<  8) | ((int32_t) rx_data[4]);
}

void tmc5272_writeRegister(uint16_t icID, uint8_t address, int32_t value, uint8_t* spi_status)
{
	uint8_t tx_data[5] = { 0 };
	uint8_t rx_data[5] = { 0 };

	tx_data[0] = address | TMC5272_WRITE_BIT;
	tx_data[1] = 0xFF & (value>>24);
	tx_data[2] = 0xFF & (value>>16);
	tx_data[3] = 0xFF & (value>>8);
	tx_data[4] = 0xFF & (value>>0);

	// Send the write request
	tmc5272_readWriteSPI(icID, &tx_data[0], sizeof(tx_data), rx_data);

	// Store status received in rx
	*spi_status = rx_data[0];
}

void tmc5272_rotateMotor(uint16_t icID, uint8_t motor, int32_t velocity)
{
  if(motor >= TMC5272_MOTORS)
		return;

	tmc5272_writeRegister(icID, TMC5272_VMAX(motor), (velocity >= 0)? velocity : -velocity, NULL);
	tmc5272_fieldWrite(icID, TMC5272_RAMPMODE_FIELD(motor), (velocity >= 0) ? TMC5272_MODE_VELPOS : TMC5272_MODE_VELNEG);
}
