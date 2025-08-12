#include "4131.h"
#include "led.h"
#include "load_cell.h"
#include "mxc_delay.h"
#include "mxc_device.h"
#include "mxc_pins.h"
#include "nextion.h"
#include "nvic_table.h"

#include "spi.h"
#include "uart.h"
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* globals */
volatile int UART_ISR_FLAG = 0; // 1 when interrupt occurs
volatile int BTN_PRESSED = 0;
#define BTN_RX_BYTES 7 // expects 7 bytes



void readCallback(mxc_uart_req_t *req, int error);

void UART1_Handler(void)
{
    MXC_UART_AsyncHandler(NEXTION_UART_REG);
}

int main(void) {
  spi_main_init();
  nextion_init();
  write_mem_map();
  read_adc_id();
  // ASYNC UART
  uint8_t rx_data[BTN_RX_BYTES];
  uint8_t tx_data[BTN_RX_BYTES];
  while(!BTN_PRESSED) {}
  NVIC_ClearPendingIRQ(UART1_IRQn);
  NVIC_DisableIRQ(UART1_IRQn);
  MXC_NVIC_SetVector(UART1_IRQn, UART1_Handler);
  NVIC_EnableIRQ(UART1_IRQn);
  // setup non blocking uart transaction request
  mxc_uart_req_t read_req = {
    .uart = NEXTION_UART_REG,
    .txData = tx_data,
    .rxData = rx_data,
    .txLen = BTN_RX_BYTES,
    .txCnt = 0,
    .rxCnt = 0,
    .callback = readCallback
  }; 
  
  UART_ISR_FLAG = 1;
  int error = 0;
  error = MXC_UART_TransactionAsync(&read_req);
  if (error != E_NO_ERROR) {
        printf("-->Error starting sync write: %d\n", error);
        printf("-->Example Failed\n");
        return error;
    }
  while (UART_ISR_FLAG) {}
  // uint32_t base0 = calibrate(LOAD_CELL_0);
  // uint32_t base1 = calibrate(LOAD_CELL_1);
  // uint32_t base2 = calibrate(LOAD_CELL_2);
  print_buff(rx_data, BTN_RX_BYTES);
  MXC_SPI_Shutdown(SPI_MAIN);
  return E_NO_ERROR;
}
