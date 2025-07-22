#ifndef _4131_H_
#define _4131_H_
// move to header file
#define ADC_STATUS 0x00 //
#define ADC_CONTROL  0x01 //
#define ADC_DATA         0x02 // 
#define ADC_IO_CONTROL   0x03
#define ADC_VBIAS_CONTROL       0x04
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
#define ADC_GAIN_x(x) (0x31+(x))
#define ADC_MISC 0x39

//* ADC Control Register (0x01), Reset: 0x4000 *//
// #define BIPOLAR 1 << 14 // bipolar
// #define REF_VAL 1 << 13 // internal ref: 1.25V
// #define CONT_READ 1 << 11 // enable
// #define DATA_STATUS 1 << 10 // append status 
// #define CSB_EN 1 << 9 // 4 wire mode
// #define REF_EN 1 << 8 // internal ref enable
// #define DUTY_CYC_RATIO 1 << 6
// #define MODE_X_MASK GENMASK(5, 2) // 0b0: cont. conversion mode by default
// #define CLK_SEL GENMASK(1, 0)

/* Satus Register */
// #define RDYB 1 << 7
// #define MAIN_ERR 1 << 6

#define WORD_SIZE 8 // Must be less than 16
#define SPI_MAIN MXC_SPI1
#define SPI_MASTER_SSIDX 0
#define VALUE 0xFF
#define SPI_SPEED 1000000 // 1 MHz
#define CS_PIN MXC_GPIO_PIN_23
#define CS_PORT MXC_GPIO0
#define TX_DATA_LEN 4

extern uint8_t main_rx[TX_DATA_LEN];
void print_buff(const uint8_t *buff, size_t len);
int spi_send_data(const uint8_t *tx_data, const uint8_t *rx_data, size_t len);
void set_reg( uint8_t reg_addr, uint8_t *data, size_t bytes);
void set_channel_0(void);
void set_channel_m(void);
void set_status(void);
void set_ctrl(void);
void write_mem_map(void);


#endif // 4131_H_