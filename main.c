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

/* UART globals */
volatile int UART_ISR_FLAG = 0; // 1 when interrupt occurs
#define BYTES 7 // expects 7 bytes
#define RX_LVL 1U<<4 // enable overrun interrupt
volatile int count = 0;
volatile uint32_t* uart_int_en, *uart_int_flags;


void readCallback(mxc_uart_req_t *req, int error);

void readCallback(mxc_uart_req_t *req, int error) {
  printf(">>>> Callback function <<<< \n\n");
  UART_ISR_FLAG = error;
  count ++; // increment each time interrupt occurs

}

void UART1_ISR(void) {
  // readback stat
  // readback flag
  // readback thresh
  printf("~~~~~~ In ISR. Flag = %d ~~~~~~\n\n", UART_ISR_FLAG);
  printf("Flags: %d \n", MXC_UART_GetFlags(NEXTION_UART_REG));
  UART_ISR_FLAG = 1;
  MXC_UART_AsyncHandler(NEXTION_UART_REG);

  
  MXC_UART_ClearFlags(NEXTION_UART_REG, 1U<<4);
  NEXTION_UART_REG->int_en = 0x10; 
  MXC_UART_ClearRXFIFO(NEXTION_UART_REG);
}

int main(void) {
    uart_int_en = (MXC_BASE_UART1 + MXC_R_UART_INT_EN);
    uart_int_flags = (MXC_BASE_UART1 + MXC_R_UART_INT_FL);
  spi_main_init();
  write_mem_map();
  read_adc_id();
  // ASYNC UART
  uint8_t rx_data[BYTES];
  uint8_t tx_data[BYTES];
  // set flag to 0
  UART_ISR_FLAG = 0;
  

  // enable interrupts
  NVIC_ClearPendingIRQ(UART1_IRQn);
  NVIC_DisableIRQ(UART1_IRQn);
  MXC_NVIC_SetVector(UART1_IRQn, UART1_ISR);
  NVIC_EnableIRQ(UART1_IRQn);
  MXC_UART_SetRXThreshold(NEXTION_UART_REG, BYTES);
  //MXC_UART_EnableInt(NEXTION_UART_REG, RX_LVL);
  NEXTION_UART_REG->int_en = 0x10;


  // uart init
  nextion_init();
  // setup async uart transaction request
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
  int error = MXC_UART_TransactionAsync(&read_req);

  // uint32_t base0 = calibrate(LOAD_CELL_0);
  // uint32_t base1 = calibrate(LOAD_CELL_1);
  // uint32_t base2 = calibrate(LOAD_CELL_2);
  
  while(1) {
    //unsigned int flags =  MXC_UART_GetFlags(NEXTION_UART_REG);
    //printf("Current flags: %X \n", flags);

    while (!UART_ISR_FLAG) {

      //uart_flags = MXC_UART_GetFlags(NEXTION_UART_REG);
      //uart_int_en = NEXTION_UART_REG->int_en;

      // poll_weights(base0, base1, base2);
      // printf("%d\n", UART_ISR_FLAG);
    }
    UART_ISR_FLAG = 0;
    print_buff(rx_data, BYTES);
    if (error != E_NO_ERROR) {
      printf("-->Error starting async read: %d\n", error);
      printf("-->Example Failed\n");
      return error;
    }
      print_buff(rx_data, BYTES);
      // NVIC_EnableIRQ(UART1_IRQn);


    }
  MXC_SPI_Shutdown(SPI_MAIN);
  return E_NO_ERROR;
}
