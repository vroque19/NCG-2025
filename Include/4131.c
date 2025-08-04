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
    // SET MODE
    MXC_SPI_SetMode(MXC_SPI1, SPI_MODE_3);
    // SET WIDTH
    MXC_SPI_SetWidth(SPI_MAIN, SPI_WIDTH_STANDARD);
    // SET GPIO FOR SPI
    const mxc_gpio_cfg_t gpio_cfg_spi1_vddioh = { MXC_GPIO1, (MXC_GPIO_PIN_26 | MXC_GPIO_PIN_28 | MXC_GPIO_PIN_23 |MXC_GPIO_PIN_29),
        MXC_GPIO_FUNC_ALT1, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIOH, MXC_GPIO_DRVSTR_0 };

    if (MXC_GPIO_Config(&gpio_cfg_spi1_vddioh) != E_NO_ERROR)
    {
        printf("\nCS PIN CONFIGURATION ERROR\n");
        while (1)
        {
        }
    }
    printf("SPI Master Initialization Complete. Speed: %d Hz\n", SPI_SPEED);
}


void test_spi_send(void)
{
    uint8_t main_rx[TX_DATA_LEN];
    uint8_t tx_buff[] = {0x02, 0x03, 0x04};
    // printf("Sending %d bytes\n", TX_DATA_LEN);
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
// prints transmitting buffer
void print_buff(const uint8_t *buff, size_t len) {
    // printf("Sending %d bytes:", len);
    for(int i = 0; i < len; i++) {
        printf(" 0x%2X ", buff[i]);
    }
    printf("\n");
}

// print received buffer in hex and decimal
void print_buff_received(const uint8_t *buff, size_t len) {
    for(int i = 0; i < len; i++) {
        printf(" 0x%2d", buff[i]);
    }
    printf("\n");
    printf("\n");
}

uint32_t hex_to_code(const uint8_t *buff, size_t len) {
    uint32_t decimal = 0;
    for(int i = 0; i < len; i++) {
        decimal = (decimal<<8) | buff[i];
    }
    // printf("\nreading: %d", decimal);
    // printf("\n");
    // printf("\n");
    return decimal;
}

int spi_send_data(const uint8_t *tx_data, const uint8_t *rx_data, size_t len)
{
   MXC_GPIO_OutClr(CS_PORT, CS_PIN); // ASSERT Chip Select (CS)
    
    // print_buff(tx_data, len);
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
    // print_buff_received(rx_data, len);
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
    // printf("Setting register 0x%2X \n", reg_addr);
    spi_send_data(tx_buff, main_rx, bytes);
}

void spi_read_reg(uint8_t *rx_data, uint8_t reg_addr, size_t bytes) {
    bytes += 1;
    uint8_t tx_buff[bytes];
    tx_buff[0] = reg_addr | 0x40; // Set MSB
    for(int i = 1; i < bytes; i++) {
        tx_buff[i] = 0x00; // Fill with zeros
    }
    uint8_t rx_buff[bytes];
    // print_buff(tx_buff, bytes);
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
    // if(reg_addr == ADC_DATA) {
    //     printf("Reading status after read"); // rdy should be 1
    //     read_status();
    // }

    memcpy(rx_data, rx_buff, bytes-1);
}

void set_misc(void) {
    size_t bytes = 2;
    uint8_t tx_data[] = {0x00, 0x00};
    set_reg(ADC_MISC, tx_data, bytes);
}

void set_gain_n(void) {
    size_t bytes = 2;
    int n = 7;
    uint8_t tx_data[] = {0x55, 0xC6};
    for(int i = 0; i <= n; i++) {
        set_reg(ADC_GAIN_X(i), tx_data, bytes);
    }
}

void set_offset_n(void) {
    size_t bytes = 2;
    uint8_t tx_data[] = {0x80, 0x00};
    int n = 7;
    for(int i = 0; i <= n; i++) {
        set_reg(ADC_OFFSET_X(i), tx_data, bytes);
    }
}

void set_channel_m(void) {
    size_t bytes = 3;
    uint8_t tx_data[] = {0x00, 0x01, 0x00};
    int m = 15;
    for(int i = 1; i <= m; i++) {
        set_reg(ADC_CHANNEL_X(i), tx_data, bytes);
    }
    return;
}

void set_config_n(void) {
    size_t bytes = 2;
    int n = 7;
    uint8_t tx_data[] = {0x0, 0x0C};
    for(int i = 0; i <= n; i++) {
        if(i > 1) {
            tx_data[1] = 0x0;
        }
        set_reg(ADC_CONFIG_X(i), tx_data, bytes);
    }
}

void set_filter_n(void) {
    size_t bytes = 3;
    int n = 7;
    // filter mode: sinc^3 standalone filter
    // ODR: 160 sps
    uint8_t tx_data[] = {0x0, 0x20, 0xA0};
    for(int i = 0; i <= n; i++) {
        if (i > 1) {
            // 48 sps
            tx_data[2] = 0x30;
        }
        set_reg(ADC_FILTER_X(i), tx_data, bytes);
    }
}

// proposed refinement

void configure_adc_channel(uint8_t channel_idx, uint8_t enable_bit) {
    uint8_t ain_m[] = {0x01, 0x43, 0x85};
    size_t bytes = 3;
    uint8_t tx_data[3];
    tx_data[0] = enable_bit;
    if(enable_bit == 0) {
        tx_data[1] = 0x01; // disable
    } else {
        tx_data[1] = ain_m[channel_idx];  // a inp
    }
    tx_data[2] = 0x00;       // Other channel specific settings (keep as 0x00 for now)

    printf("Configuring ADC_CHANNEL_X(%d) with [0x%02X, 0x%02X, 0x%02X]\n",
           channel_idx, tx_data[0], tx_data[1], tx_data[2]);
    set_reg(ADC_CHANNEL_X(channel_idx), tx_data, bytes);
}

uint32_t get_data_from_channel(uint8_t channel_idx) {
    uint8_t rx_data[3];
    uint32_t code = 0;
    size_t bytes = 3;

    configure_adc_channel(channel_idx, 0x80);
    MXC_Delay(MXC_DELAY_MSEC(500));
    // Poll ADC Status
    uint8_t status_byte[1];
    int timeout_ms = 500; // Max time to wait for data (adjust as needed)
    int elapsed_ms = 0;

    printf("Waiting for data ready on channel %d...\n", channel_idx);
    do {
        spi_read_reg(status_byte, ADC_STATUS, 1); // Read 1 byte from Status register
        // printf("Status: 0x%02X\n", status_byte[0]); // For debugging
        if (status_byte[0] & 0x01) { // Example: assuming bit 0 of status indicates data ready
            printf("Data ready!\n");
            break;
        }
        MXC_Delay(MXC_DELAY_USEC(500)); // Small delay between status polls
        elapsed_ms += 1; // Assuming 0.5ms delay for simplicity. Adjust if needed.
    } while (elapsed_ms < timeout_ms * 2); // Poll for timeout_ms (2 * 0.5ms/poll)

    if (elapsed_ms >= timeout_ms * 2) {
        printf("Timeout waiting for ADC data on channel %d!\n", channel_idx);
        return 0; // Or handle error appropriately
    }
    // Step 3: Read the ADC data
    spi_read_reg(rx_data, ADC_DATA, bytes);
    code = hex_to_code(rx_data, bytes);

    printf("Read from Channel %d: Code = %d\n\n", channel_idx, code);
    configure_adc_channel(channel_idx, 0x00);
    return code;

}

void set_channel_0(void) {
    size_t bytes = 3;
    uint8_t tx_data[] = {0x80, 0x01, 0x00};
    set_reg(ADC_CHANNEL_X(0), tx_data, bytes);
    // uint8_t disable[] = {0x00, 0x01, 0x00};
    // set_reg(ADC_CHANNEL_X(1), disable, bytes);
    // set_reg(ADC_CHANNEL_X(2), disable, bytes);
}

void set_channel_1(void) {
    size_t bytes = 3;
    uint8_t tx_data[] = {0x80, 0x43, 0x00};
    set_reg(ADC_CHANNEL_X(1), tx_data, bytes);
    uint8_t disable[] = {0x00, 0x01, 0x00};
    set_reg(ADC_CHANNEL_X(0), disable, bytes);
    // set_reg(ADC_CHANNEL_X(2), disable, bytes);
    
}

void set_channel_2(void) {
    size_t bytes = 3;
    uint8_t tx_data[] = {0x80, 0x85, 0x00};
    set_reg(ADC_CHANNEL_X(2), tx_data, bytes);
}



void set_ctrl(void) {
    size_t bytes = 2;
    uint8_t tx_data[] = {0x01, 0x02};
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
    // set_ctrl();
    // set_data();
    set_io_ctrl();
    set_vbias_ctrl();
    // set_mclk_count();
    // set_channel_0();
    // set_channel_m();
    set_config_n();
    set_filter_n();
    set_offset_n();
    set_gain_n();
    set_misc();
    printf("\n >>> Successfully set the 4131 registers << \n\n");
}

void read_adc_id(void) {
    size_t bytes = 2;
    uint8_t rx_data[bytes];
    spi_read_reg(rx_data, ADC_ID, bytes);
    print_buff_received(rx_data, bytes);
    
}

void read_adc_conversion(void) {
    printf("repeating....\n");
    size_t bytes = 3;
    uint8_t rx_data[] = {0x0, 0x0, 0x0}; 
    // rx_data will filled with data from data reg
    spi_read_reg(rx_data, ADC_DATA, bytes);
    uint32_t code = 0;

    // print_buff_received(rx_data, bytes);
    code = hex_to_code(rx_data, bytes);
    printf("\nThis is my code: %d\n", code);

}

uint8_t read_status(void) {
    size_t bytes = 2;
    uint8_t rx_data[bytes];
    spi_read_reg(rx_data, ADC_STATUS, bytes);
    printf("Status Register:\n");
    print_buff_received(rx_data, bytes);
    return rx_data[0];
}

uint32_t get_adc_data(void) {
    size_t bytes = 3;
    uint8_t rx_data[bytes];
    spi_read_reg(rx_data, ADC_DATA, bytes);
    uint32_t code = 0;
    code = hex_to_code(rx_data, bytes);
    return code;

}
