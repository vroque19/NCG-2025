/* 
MIT License

Copyright (c) 2025 Analog Devices

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef _4131_H_
#define _4131_H_
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include "mxc_delay.h"
#include "spi.h"

#define ADC_STATUS 0x00 //
#define ADC_CONTROL  0x01 //
#define ADC_DATA         0x02 // 
#define ADC_IO_CONTROL   0x03
#define ADC_VBIAS_CONTROL       0x04
// 16-Bit WLCSP Model ID. These bits are set by default for each model and are read only.
#define ADC_ID           0x05
#define ADC_ERROR        0x06
#define ADC_ERROR_EN     0x07
#define ADC_MCLK_COUNT   0x08
// The channel
// for each channel, a predefined ADC setup can be selected
#define ADC_CHANNEL_X(x) (0x09+(x))
// enable channel active high
// Each ADC setup consists of config, filter, gain and offset
#define ADC_CONFIG_X(x) (0x19+(x))
#define ADC_FILTER_X(x) (0x21+(x))
#define ADC_OFFSET_X(x) (0x29+x)
#define ADC_GAIN_X(x) (0x31+(x))
#define ADC_MISC 0x39

#define WORD_SIZE 8
#define SPI_MAIN MXC_SPI1
#define SPI_MASTER_SSIDX 0
#define VALUE 0xFF
#define SPI_SPEED 10000 // 10k
#define CS_PIN MXC_GPIO_PIN_23
#define CS_PORT MXC_GPIO0
#define TX_DATA_LEN 4

void write_mem_map(void);
void spi_main_init(void);
void configure_adc_channel(uint8_t channel_idx, uint8_t enable_bit);
void set_reg( uint8_t reg_addr, uint8_t *data, size_t bytes);
void set_status(void);
void set_ctrl(void);
void set_config_n(void);
void set_filter_n(void);
void set_offset_n(void);
void set_gain_n(void);
void set_misc(void);
void spi_read_reg(uint8_t* rx_data, uint8_t reg_addr, size_t bytes);
uint16_t bytes_to_dec(uint8_t *bytes);
uint16_t hex_to_code(const uint8_t *buff, size_t len);
uint16_t get_data_from_channel(uint8_t channel_idx);
void read_adc_id(void);
void read_adc_conversion(void);
uint16_t get_adc_data(void);
uint8_t read_status(void);

int spi_send_data(const uint8_t *tx_data, const uint8_t *rx_data, size_t len);
void test_spi_send(void);
void print_buff(const uint8_t *buff, size_t len);
#endif // 4131_H_