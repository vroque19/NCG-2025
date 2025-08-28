#ifndef TMC5272_SPI_
#define TMC5272_SPI_

/* TMC5272 SPI Protocol

The TMC5272 TMC-API defines the following extern functions for impementation on
our MAX32650 MCU. 

This is the implementation of these functions.

*/

#include <stddef.h>
#include "spi.h"
#include "mxc_device.h"
#include "mxc_pins.h"

/***** Definitions *****/
#define DATA_SIZE 8
#define VALUE 0xFF
#define SPI_SPEED 10000 // Bit Rate (Max.: 1,850,000)
#define SPI_MASTER_SS_INDEX 1      // SS0 = P1_23

/* Function Prototypes */
void tmc5272_readWriteSPI(uint16_t icID, uint8_t* tx_data, uint8_t dataLength, uint8_t* rx_data);
uint8_t tmc5272_getNodeAddress(uint16_t icID);

#endif