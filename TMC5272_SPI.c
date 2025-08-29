#include "TMC5272_SPI.h"

void tmc5272_SPI_init(tmc5272_dev_t* tmc5272_dev)
{
    /***** Configure master *****/
    // Copy cfg to a new struct (MSDK uses const)
    mxc_gpio_cfg_t spi_port_cfg = *(tmc5272_dev->gpio_cfg_spi_port);
    
    // Combine pin mask for MOSI/MISO/CLK and SSn pin
    const mxc_gpio_cfg_t* ss_port_cfg = tmc5272_dev->gpio_cfg_spi_ss;
    spi_port_cfg.mask |= ss_port_cfg->mask;
    
    // Set VDDIOH (3.3V)
    spi_port_cfg.vssel = MXC_GPIO_VSSEL_VDDIOH;
    MXC_GPIO_Config(tmc5272_dev->gpio_cfg_spi_ss);
    
    /*** Try to initialize ***/
    if (MXC_SPI_Init(tmc5272_dev->spi_port, 1, 0, 1, (0 << tmc5272_dev->ss_pin_index), SPI_SPEED) != E_NO_ERROR) {
        //printf("\nSPI MASTER INITIALIZATION ERROR\n");
        while (1) {}
    }
    
    MXC_SPI_SetDataSize(tmc5272_dev->spi_port, 8); // 8 bits per byte
    MXC_SPI_SetWidth(tmc5272_dev->spi_port, SPI_WIDTH_STANDARD);
    MXC_SPI_SetMode(tmc5272_dev->spi_port, SPI_MODE_3);
}


void tmc5272_readWriteSPI(tmc5272_dev_t* tmc5272_dev, uint8_t* tx_data, uint8_t dataLength, uint8_t* rx_data)
{
    /***** Initialize Transaction Parameters *****/
    mxc_spi_req_t master_req;
    master_req.spi = tmc5272_dev->spi_port;
    master_req.txData = tx_data;
    master_req.rxData = rx_data;
    master_req.txLen = dataLength;
    master_req.rxLen = dataLength;
    master_req.ssIdx = tmc5272_dev->ss_pin_index;
    master_req.ssDeassert = 1;
    master_req.txCnt = 0;
    master_req.rxCnt = 0;
    master_req.completeCB = NULL;

    /***** Perform Transaction *****/
    MXC_SPI_MasterTransaction(&master_req);
    return;
}