#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include "mxc_device.h"
#include "mxc_delay.h"
#include "mxc_pins.h"
#include "uart.h"
#include "led.h"
#include "spi.h"
#include "include/4131.h"
extern uint8_t main_rx[TX_DATA_LEN];
// uint8_t main_rx[TX_DATA_LEN];
// void ADC_SPI_Init(void);
void spi_main_init(void);


void spi_main_init(void)
{
    // INIT SPI
    if (MXC_SPI_Init(SPI_MAIN, 1, 0, 1, 0, SPI_SPEED) != E_NO_ERROR)
    {
        printf("\nSPI MASTER INITIALIZATION ERROR\n");
        while (1)
        {
        }
    }
    // SET DATA SIZE
    MXC_SPI_SetDataSize(SPI_MAIN, WORD_SIZE);
    // SET MDOE
    MXC_SPI_SetMode(MXC_SPI1, SPI_MODE_3);
    // SET GPIO FOR SPI
    const mxc_gpio_cfg_t gpio_cfg_spi1_vddioh = { MXC_GPIO1, (MXC_GPIO_PIN_26 | MXC_GPIO_PIN_28 | MXC_GPIO_PIN_23 |MXC_GPIO_PIN_29),
        MXC_GPIO_FUNC_ALT1, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIOH, MXC_GPIO_DRVSTR_0 };
    // SET CS PIN
    mxc_gpio_cfg_t cs_pin_config = {
        .port = CS_PORT,
        .mask = CS_PIN,
        .pad = MXC_GPIO_PAD_NONE,
        .func = MXC_GPIO_FUNC_ALT1,
        .vssel = MXC_GPIO_VSSEL_VDDIOH, // Set voltage to 3.3V
        .drvstr = MXC_GPIO_DRVSTR_0     // Default drive strength
    };
    // CHECK FOR ERRORS
    if (MXC_GPIO_Config(&cs_pin_config) != E_NO_ERROR)
    {
        printf("\nCS PIN CONFIGURATION ERROR\n");
        while (1)
        {
        }
    }
    if (MXC_GPIO_Config(&gpio_cfg_spi1_vddioh) != E_NO_ERROR)
    {
        printf("\nCS PIN CONFIGURATION ERROR\n");
        while (1)
        {
        }
    }
    // DEASSERT CS (PULL HIGH)
    //MXC_GPIO_OutSet(CS_PORT, CS_PIN);
    printf("SPI Master Initialization Complete. Speed: %d Hz\n", SPI_SPEED);
}

// TODO Configure the AD4131
/*

*/

void test_spi_send(void)
{
    uint8_t tx_buff[] = {0x02, 0x03, 0x04};
    printf("Sending %d bytes\n", TX_DATA_LEN);
    printf("\n");
    int result = spi_send_data(tx_buff, main_rx, 3);
    if (result != E_NO_ERROR)
    {
        printf("ERROR\n");
    }
    else
    {
        printf("\n");
    }
}

int main(void)
{
    // SPI transaction request
    spi_main_init();
    write_mem_map();

    MXC_SPI_Shutdown(SPI_MAIN);
    return E_NO_ERROR;
}