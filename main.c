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
#define ADC_STATUS 0x00
#define ADC_ADC_CONTROL  0x01
#define ADC_DATA         0x02
#define ADC_IO_CONTROL   0x03
#define ADC_VBIAS_CONTROL       0x04
#define ADC_ID           0x05
#define ADC_ERROR        0x06
#define ADC_ERROR_EN     0x07
#define ADC_MCLK_COUNT   0x08
// The channel
// for each channel, a predefined ADC setup can be selected
#define ADC_CHANNEL_X(x) (0x09+(x))
// Each ADC setup consists of config, filter, gain and offset
#define ADC_CONFIG_X(x) (0x19+(x))
#define ADC_FILTER_X(x) (0x21+(x))
#define ADC_OFFSET_X(x) (0x29+x)
#define ADC_GAIN_x(x) (0x31+(x))
#define ADC_MISC 0x39

//* ADC Control Register (0x01), Reset: 0x4000 *//
#define BIPOLAR 1 << 14 // bipolar
#define REF_VAL 1 << 13 // internal ref: 1.25V
#define CONT_READ 1 << 11 // enable
#define DATA_STATUS 1 << 10 // append status 
#define CSB_EN 1 << 9 // 4 wire mode
#define REF_EN 1 << 8 // internal ref enable
#define DUTY_CYC_RATIO 1 << 6
#define MODE_X_MASK GENMASK(5, 2) // 0b0: cont. conversion mode by default
#define CLK_SEL GENMASK(1, 0)

/* Satus Register */
#define RDYB 1 << 7
#define MAIN_ERR 1 << 6
#define WORD_SIZE 8 
#define SPI_MAIN MXC_SPI1 
#define SPI_MASTER_SSIDX 0
#define VALUE 0xFF
#define SPI_SPEED 60000000 // 60 MHz 
#define CS_PIN MXC_GPIO_PIN_23
#define CS_PORT MXC_GPIO0
// TEST DATA
#define TX_DATA_LEN 24 // data can be 8, 16, or 24 bit
uint8_t master_rx[TX_DATA_LEN];
uint8_t master_tx[TX_DATA_LEN];

// Global SPI request
mxc_spi_req_t master_req;
// FUNCTION PROTOTYPES
void ADC_SPI_Init(void);
void spi_main_init(void);
int spi_main_send_data(const uint8_t *tx_data, uint8_t *rx_data, size_t len);


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
// void ADC_SPI_Init(void) {
//     // init SPI as Master
//     if (MXC_SPI_Init (SPI_MASTER, 1, 0, 1, 0, SPI_SPEED) != E_NO_ERROR) {
//         printf("\nSPI MASTER INITIALIZATION ERROR\n");
//         while (1) {}
//     }                     
//     // MXC_GPIO_Config(&gpio_cfg_spi1_ss0);
//     // Set data size to hold up to 24 bits
//     MXC_SPI_SetDataSize(SPI_MASTER, DATA_SIZE);
//     MXC_SPI_SetWidth(SPI_MASTER, SPI_WIDTH_STANDARD);

//     // mxc_gpio_regs_t spi_reg;
//     MXC_SPI_SetMode(MXC_SPI1, SPI_MODE_3);
//     mxc_gpio_cfg_t SPI_CS = { // config GPIO CS P0.23
//         .drvstr = MXC_GPIO_DRVSTR_0,
//         .port = MXC_GPIO0,
//         .func = MXC_GPIO_FUNC_OUT,
//         .mask = MXC_GPIO_PIN_23,
//         .vssel = MXC_GPIO_VSSEL_VDDIOH,
//         .pad = MXC_GPIO_PAD_NONE
//     };
//     if (MXC_GPIO_Config(&SPI_CS) != E_NO_ERROR) {
//         printf("\nCS PIN CONFIGURATION ERROR\n");
//         while(1){}
//     }
    
//     // MXC_GPIO_OutClr(MXC_GPIO_PORT_1, MXC_GPIO_PIN_23); // set pin LOW
//     // MXC_GPIO_OutSet(MXC_SPI1, MXC_GPIO_PIN_23); // set pin HIGH
//     printf("Initialization of SPI complete.\n\n");
// }

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
    MXC_SPI_Shutdown(SPI_MAIN);
    return E_NO_ERROR;
   
}