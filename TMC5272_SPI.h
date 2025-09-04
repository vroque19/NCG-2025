#ifndef TMC5272_SPI_
#define TMC5272_SPI_

/* TMC5272 SPI Protocol

The TMC5272 TMC-API defines the following extern functions for impementation on
our MAX32650 MCU. 

This is the implementation of these functions.

*/

#include <stddef.h>
#include <stdio.h>
#include "spi.h"
#include "mxc_device.h"
#include "mxc_pins.h"

/***** Definitions *****/
#define SPI_SPEED           10000   // Bit Rate (Max.: 1,850,000)
#define TMC5272_SPI_SIZE    5       // Bytes per transaction
#define SPI_NUM_CLIENTS     3       // Max SS line being used 

/* Data Structures */
typedef struct  {
    mxc_spi_regs_t* spi_port;                   // SPI Port
    mxc_gpio_cfg_t* gpio_cfg_spi;          // GPIO Config for SPI port (incl. all SS lines)
    uint8_t ss_index;                           // SPI SS pin index
} tmc5272_dev_t;

/* Function Prototypes */
void tmc5272_SPI_init(tmc5272_dev_t* tmc5272_dev);
void tmc5272_SPI_readWrite(tmc5272_dev_t* tmc5272_dev, uint8_t* tx_data, uint8_t dataLength, uint8_t* rx_data);

#endif