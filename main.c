#include "4131.h"
#include "load_cell.h"
#include "mxc_pins.h"
#include "nextion.h"

#include <stddef.h>


// volatile int8_t rx_data[BYTES];

volatile int UART_ISR_FLAG = 0;
typedef enum {
    MANUAL_MODE,
    TOUCHSCREEN_MODE,
    AUTOMATED_MODE
} project_mode_t;

int main(void) {
  spi_main_init();
  write_mem_map();
  read_adc_id();
  // uart init
  nextion_init();
  // ASYNC UART
  uint8_t rx_data[BYTES];
  uint8_t tx_data[BYTES];
  // uart interrupt init
  nextion_int_init();
    // // setup async uart transaction request
    mxc_uart_req_t read_req = {
        .uart = NEXTION_UART_REG,
        .txData = tx_data,
        .rxData = rx_data,
        .txLen = BYTES,
        .rxLen = BYTES,
        .txCnt = 0,
        .rxCnt = 0,
        .callback = readCallback
    }; 
    MXC_UART_TransactionAsync(&read_req);

  uint32_t base0 = calibrate(LOAD_CELL_0);
  uint32_t base1 = calibrate(LOAD_CELL_1);
  uint32_t base2 = calibrate(LOAD_CELL_2);
  
  while(1) {
    while (!UART_ISR_FLAG) {
      MXC_UART_TransactionAsync(&read_req); // rearm uart interrupt
    }
    UART_ISR_FLAG = 0;
    print_buff(rx_data, BYTES);

    }
  MXC_SPI_Shutdown(SPI_MAIN);
  return E_NO_ERROR;
}
