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
#include "AD4131.h"

#define WORD_SIZE 8 
#define SPI_MAIN MXC_SPI1 
#define SPI_MASTER_SSIDX 0
#define VALUE 0xFF
#define SPI_SPEED 6000000 // 60 MHz 
#define CS_PIN MXC_GPIO_PIN_23
#define CS_PORT MXC_GPIO0
// TEST DATA
#define TX_DATA_LEN 16
uint8_t main_rx[TX_DATA_LEN];
uint8_t main_tx[TX_DATA_LEN];


// FUNCTION PROTOTYPES
void ADC_SPI_Init(void);
void spi_main_init(void);
// int spi_send_data(const uint8_t *tx_data, const uint8_t *rx_data, size_t len);


void spi_main_init(void) {
    // INIT SPI
    if (MXC_SPI_Init (SPI_MAIN, 1, 0, 1, 0, SPI_SPEED) != E_NO_ERROR) {
        printf("\nSPI MASTER INITIALIZATION ERROR\n");
        while (1) {}
    }
    // SET DATA SIZE
    MXC_SPI_SetDataSize(SPI_MAIN, WORD_SIZE);
    // SET MDOE
    MXC_SPI_SetMode(MXC_SPI1, SPI_MODE_3);
    // SET CS PIN
    mxc_gpio_cfg_t cs_pin_config = {
        .port = CS_PORT,
        .mask = CS_PIN,
        .pad = MXC_GPIO_PAD_NONE,
        .func = MXC_GPIO_FUNC_OUT,
        .vssel = MXC_GPIO_VSSEL_VDDIOH, // Set voltage to 3.3V
        .drvstr = MXC_GPIO_DRVSTR_0     // Default drive strength
    };
    if (MXC_GPIO_Config(&cs_pin_config) != E_NO_ERROR) {
        printf("\nCS PIN CONFIGURATION ERROR\n");
        while(1){}
    }
    // DEASSERT CS (PULL HIGH)
    MXC_GPIO_OutSet(CS_PORT, CS_PIN);
    printf("SPI Master Initialization Complete. Speed: %d Hz\n", SPI_SPEED);
}

int spi_send_data(const uint8_t *tx_data, const uint8_t *rx_data, size_t len) {
    // ASSERT CS
    MXC_GPIO_OutClr(CS_PORT, CS_PIN);
    MXC_Delay(MXC_DELAY_USEC(1));
    // INITIALIZE MAIN REQUEST PARAMETERS
    mxc_spi_req_t main_req;
    main_req.spi = SPI_MAIN;
    main_req.txLen = len;
    main_req.txCnt = 2;
    main_req.txData = (uint8_t*)tx_data;
    main_req.rxLen = len;
    main_req.rxData = (uint8_t*)rx_data;
    main_req.rxCnt = 0;
    main_req.completeCB = NULL;
    main_req.ssIdx = 0;
    main_req.ssDeassert = 1;
    // DEASSERT CS
    // COMPLETE TRANSACTION
    int res = MXC_SPI_MasterTransaction(&main_req);
    MXC_GPIO_OutSet(CS_PORT, CS_PIN);
    MXC_Delay(MXC_DELAY_MSEC(200));
    if(res != E_NO_ERROR) {
        printf("Error in transaction %d\n", res);
    }
    return res;
}


// Configure the AD4131
/*
1. Select wire mode
 - write to ADC_Control Reg
2. Setup configuration
 - write to config_n and filter_n
3. Channel configuration
  - write to the CHANNEL_m registers
4. Setup ADC mode
 - write to ARC_Control Reg
*/


int main(void) {
    // SPI transaction request
    // ADC_SPI_Init();
    spi_main_init();
    for (int i = 0; i < TX_DATA_LEN; i++) {
        main_tx[i] = i;
    }
    memset(main_rx, 0xFF, TX_DATA_LEN);
    while(1) {
        printf("Sending %d bytes\n", TX_DATA_LEN);
        for(int i = 0; i < TX_DATA_LEN; i++) {
            printf("0x%02X ", main_tx[i]);
        }
        printf("\n");
        int result = spi_send_data(main_tx, main_rx, TX_DATA_LEN);
        if(result != E_NO_ERROR) {
            printf("ERROR\n");
        }
        else {
            printf("SUCCESS! Receiving... \n");
            for(int i = 0; i < TX_DATA_LEN; i++) {
                printf("0x%02X ", main_rx[i]);
            }
             printf("\n");
        }
        MXC_Delay(MXC_DELAY_MSEC(100));
    }

    MXC_SPI_Shutdown(SPI_MAIN);
    return E_NO_ERROR;
   
}