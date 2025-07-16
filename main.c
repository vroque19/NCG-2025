#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "mxc_device.h"
#include "mxc_delay.h"
#include "mxc_pins.h"
#include "uart.h"
#include "spi.h"

#define DATA_LEN 1024 // Words
#define DATA_SIZE 8
#define VALUE 0xFF
#define SPI_SPEED 100000

#define SPI_MASTER MXC_SPI1
#define SPI_MASTER_SSIDX 0
#define SPI_SLAVE MXC_SPI1
#define SPI_SLAVE_SSIDX 0
#define SPI_SLAVE_IRQ SPI1_IRQn


void Master_Slave_Config(void) {
     /***** Configure master *****/
    MXC_GPIO_Config(&gpio_cfg_spi1_ss0);
    if (MXC_SPI_Init(SPI_MASTER, 1, 0, 1, 1, SPI_SPEED) != E_NO_ERROR) {
        printf("\nSPI MASTER INITIALIZATION ERROR\n");
        while (1) {}
    }
    MXC_SPI_SetDataSize(SPI_MASTER, 16);
     /***** Configure slave *****/
    MXC_SPI_SetWidth(SPI_MASTER, SPI_WIDTH_STANDARD);
    MXC_GPIO_Config(&gpio_cfg_spi1_ss0);
    if (MXC_SPI_Init(SPI_SLAVE, 0, 0, 1, (1 << SPI_SLAVE_SSIDX), SPI_SPEED) != E_NO_ERROR) {
        printf("\nSPI SLAVE INITIALIZATION ERROR\n");
        while (1) {}
    }
    MXC_SPI_SetDataSize(SPI_SLAVE, DATA_SIZE);
    MXC_SPI_SetWidth(SPI_SLAVE, SPI_WIDTH_STANDARD);
    printf("Initalization of Master and Slave complete.\n");
}

int main(void) {
    // mxc_spi_req_t master_req;
    // mxc_spi_req_t slave_req;
    Master_Slave_Config();
   


}