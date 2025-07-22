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
#include "4131.h"
uint8_t main_rx[TX_DATA_LEN];

  
void print_buff(const uint8_t *buff, size_t len) {
    printf("Sending ");
    for(int i = 0; i < len; i++) {
        printf(" 0x%2X ", buff[i]);
        MXC_Delay(MXC_DELAY_USEC(100));
    }
    printf("\n");
}

int spi_send_data(const uint8_t *tx_data, const uint8_t *rx_data, size_t len)
{
   // MXC_GPIO_OutClr(CS_PORT, CS_PIN); // ASSERT Chip Select (CS)
    
    print_buff(tx_data, len);
    // INITIALIZE MAIN REQUEST PARAMETERS
    mxc_spi_req_t main_req;
    main_req.spi = SPI_MAIN;
    main_req.txLen = len;
    main_req.txData = (uint8_t *)tx_data;
    main_req.txCnt = 0;
    main_req.rxData = (uint8_t *)rx_data;
    main_req.rxLen = len;
    main_req.rxCnt = 0;
    main_req.completeCB = NULL;
    main_req.ssIdx = 0;
    main_req.ssDeassert = 1;
    // COMPLETE TRANSACTION
    int res = MXC_SPI_MasterTransaction(&main_req);
    
    //MXC_GPIO_OutSet(CS_PORT, CS_PIN); // DEASSERT CS
    //MXC_Delay(200);
    if (res != E_NO_ERROR)
    {
        printf("Error in transaction %d\n", res);
    }
    return res;
}
void write_mem_map(void) {
    set_ctrl();
    set_status();
    set_channel_0();
    set_channel_m();
}

void set_reg( uint8_t reg_addr, uint8_t *data, size_t bytes) {
    uint8_t tx_buff[bytes];
    uint8_t command = reg_addr & (0x7F);
    tx_buff[0] = command;
    bytes += 1;
    for(int i = 0; i < bytes; i++) {
        tx_buff[i+1] = data[i];
    }
    printf("Setting register 0x%2X \n", reg_addr);
    spi_send_data(tx_buff, main_rx, bytes+1);
    return;
}

void set_channel_m(void) {
    size_t bytes = 3;
    uint8_t tx_data[] = {0x00, 0x01, 0x00};
    for(int i = 1; i < 9; i++) {
        set_reg(ADC_CHANNEL_X(i), tx_data, bytes);
    }
    return;

}

void set_channel_0(void) {
    size_t bytes = 3;
    uint8_t tx_data[] = {0x08, 0x01, 0x00};
    set_reg(ADC_CHANNEL_X(0), tx_data, bytes);
    return;
}

void set_status(void) {
    size_t bytes = 1;
    uint8_t tx_data[] = {0x90};
    set_reg(ADC_STATUS, tx_data, bytes);
    return;
}   

void set_ctrl(void) {
    size_t bytes = 2;
    uint8_t tx_data[] = {0x01, 0x00};
    set_reg(ADC_CONTROL, tx_data, bytes);
    return;
}