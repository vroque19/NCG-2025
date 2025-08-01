#include "TMC5272_SPI.h"

void tmc5272_readWriteSPI(uint16_t icID, uint8_t* tx_data, size_t dataLength, uint8_t* rx_data)
{
    // Note: Function does not use icID!


    /***** Configure master *****/
    MXC_GPIO_Config(&gpio_cfg_spi1_ss0);
    if (MXC_SPI_Init(MXC_SPI1, 1, 0, 1, (0 << SPI_MASTER_SS_INDEX), SPI_SPEED) != E_NO_ERROR) {
        //printf("\nSPI MASTER INITIALIZATION ERROR\n");
        while (1) {}
    }

    MXC_SPI_SetDataSize(MXC_SPI1, DATA_SIZE);
    MXC_SPI_SetWidth(MXC_SPI1, SPI_WIDTH_STANDARD);
    MXC_SPI_SetMode(MXC_SPI1, SPI_MODE_3);
    
    // Set 3.3V for SPI pins
    MXC_GPIO_SetVSSEL(MXC_GPIO1, 
                      MXC_GPIO_VSSEL_VDDIOH, 
                      (MXC_GPIO_PIN_23 | MXC_GPIO_PIN_26 | MXC_GPIO_PIN_28 | MXC_GPIO_PIN_29));


    /***** Initialize Transaction Parameters *****/
    mxc_spi_req_t master_req;
    master_req.spi = MXC_SPI1;
    master_req.txData = tx_data;
    master_req.rxData = rx_data;
    master_req.txLen = dataLength;
    master_req.rxLen = dataLength;
    master_req.ssIdx = SPI_MASTER_SS_INDEX;
    master_req.ssDeassert = 1;
    master_req.txCnt = 0;
    master_req.rxCnt = 0;
    master_req.completeCB = NULL;

    /***** Perform Transaction *****/
    MXC_SPI_MasterTransaction(&master_req);
    return;
}

uint8_t tmc5272_getNodeAddress(uint16_t icID) 
{
    return 0;
}