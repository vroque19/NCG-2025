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
        // MXC_Delay(MXC_DELAY_USEC(100));
    }
    printf("\n");
    printf("\n");
}

void print_buff_received(const uint8_t *buff, size_t len) {
    printf("Received ");
    for(int i = 0; i < len; i++) {
        printf(" 0x%2X ", buff[i]);
        // MXC_Delay(MXC_DELAY_USEC(100));
    }
    printf("\n");
    printf("\n");
}

int spi_send_data(const uint8_t *tx_data, const uint8_t *rx_data, size_t len)
{
   MXC_GPIO_OutClr(CS_PORT, CS_PIN); // ASSERT Chip Select (CS)
    
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
    if (res != E_NO_ERROR)
    {
        printf("Error in transaction %d\n", res);
    }
    print_buff_received(rx_data, len);
    return res;
}

void set_reg( uint8_t reg_addr, uint8_t *data, size_t bytes) {
    bytes += 1;
    uint8_t tx_buff[bytes];
    uint8_t command = reg_addr & (0x7F);
    tx_buff[0] = command;
    for(int i = 0; i < bytes; i++) {
        tx_buff[i+1] = data[i];
    }
    printf("Setting register 0x%2X \n", reg_addr);
    spi_send_data(tx_buff, main_rx, bytes);
}

void read_reg(uint8_t reg_addr, size_t bytes) {
    bytes += 1;
    uint8_t tx_buff[bytes];
    tx_buff[0] = reg_addr | 0x40; // Set MSB
    for(int i = 1; i < bytes; i++) {
        tx_buff[i] = 0x00; // Fill with zeros
    }
    uint8_t rx_buff[bytes];
    print_buff(tx_buff, bytes);
    // INITIALIZE MAIN REQUEST PARAMETERS
    mxc_spi_req_t main_req;
    main_req.spi = SPI_MAIN;
    main_req.txLen = bytes;
    main_req.txData = (uint8_t *)tx_buff;
    main_req.txCnt = 0;
    main_req.rxData = (uint8_t *)rx_buff;
    main_req.rxLen = bytes;
    main_req.rxCnt = 0;
    main_req.completeCB = NULL;
    main_req.ssIdx = 0;
    main_req.ssDeassert = 1;
    // COMPLETE TRANSACTION
    int res = MXC_SPI_MasterTransaction(&main_req);
    //MXC_GPIO_OutSet(CS_PORT, CS_PIN); // DEASSERT CS
    if (res != E_NO_ERROR)
    {
        printf("Error in transaction %d\n", res);
    }
    
    // memcpy(data, &rx_buff[1], bytes - 1);
    // print_buff_received(rx_buff, bytes-1);
    printf("Reading register 0x%2X \n", reg_addr);
    print_buff_received(&rx_buff[1], bytes-1);
}

void set_misc(void) {
    size_t bytes = 2;
    uint8_t tx_data[] = {0x00, 0x00};
    set_reg(ADC_MISC, tx_data, bytes);
}

void set_gain_n(void) {
    size_t bytes = 2;
    int n = 8;
    uint8_t tx_data[] = {0x55, 0xC6};
    for(int i = 0; i < n; i++) {
        set_reg(ADC_GAIN_X(i), tx_data, bytes);
    }
}

void set_offset_n(void) {
    size_t bytes = 2;
    uint8_t tx_data[] = {0x80, 0x00};
    int n = 2;
    for(int i = 0; i < n; i++) {
        set_reg(ADC_OFFSET_X(i), tx_data, bytes);
    }
}

void set_channel_m(void) {
    size_t bytes = 3;
    uint8_t tx_data[] = {0x00, 0x01, 0x00};
    int m = 9;
    for(int i = 1; i < m; i++) {
        set_reg(ADC_CHANNEL_X(i), tx_data, bytes);
    }
    return;
}

void set_config_n(void) {
    size_t bytes = 2;
    int n = 2;
    uint8_t tx_data[] = {0x0, 0x0C};
    for(int i = 0; i < n; i++) {
        if(i > 1) {
            tx_data[1] = 0x0;
        }
        set_reg(ADC_CONFIG_X(i), tx_data, bytes);
    }
}

void set_filter_n(void) {
    size_t bytes = 3;
    int n = 8;
    uint8_t tx_data[] = {0x0, 0x20, 0xA0};
    for(int i = 0; i < n; i++) {
        if (i > 1) {
            tx_data[2] = 0x30;
        }
        set_reg(ADC_FILTER_X(i), tx_data, bytes);
    }
}

void set_channel_0(void) {
    size_t bytes = 3;
    uint8_t tx_data[] = {0x08, 0x01, 0x00};
    set_reg(ADC_CHANNEL_X(0), tx_data, bytes);
}

void set_status(void) {
    size_t bytes = 1;
    uint8_t tx_data[] = {0x90};
    set_reg(ADC_STATUS, tx_data, bytes);
}   

void set_ctrl(void) {
    size_t bytes = 2;
    uint8_t tx_data[] = {0x01, 0x00};
    set_reg(ADC_CONTROL, tx_data, bytes);
}

void set_data(void) {
    size_t bytes = 2;
    uint8_t tx_data[] = {0x00, 0x00};
    set_reg(ADC_DATA, tx_data, bytes);
}

void set_io_ctrl(void) {
    size_t bytes = 2;
    uint8_t tx_data[] = {0x00, 0x00};
    set_reg(ADC_IO_CONTROL, tx_data, bytes);
}
void set_vbias_ctrl(void) {
    size_t bytes = 2;
    uint8_t tx_data[] = {0x00, 0x00};
    set_reg(ADC_VBIAS_CONTROL, tx_data, bytes);
}

void set_mclk_count(void) {
    size_t bytes = 1;
    set_reg(ADC_MCLK_COUNT, 0x00, bytes);
}

void write_mem_map(void) {
    // write to 4131 registers to configure for AIN0-AIN5
    // set_status();
    set_ctrl();
    set_data();
    set_io_ctrl();
    set_vbias_ctrl();
    // set_mclk_count();
    set_channel_0();
    set_channel_m();
    set_config_n();
    set_filter_n();
    set_gain_n();
    set_misc();
}

void read_adc_id(void) {
    size_t bytes = 2;
    read_reg(0x05, bytes);
    // uint8_t tx_buff[] = {0x85, 0x00, 0x00};

    // spi_send_data(tx_buff, main_rx, bytes);
}


