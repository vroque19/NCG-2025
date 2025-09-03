#include "4131.h"
#include "load_cell.h"
#include "mode_touchscreen.h"
#include "mxc_pins.h"
#include "nextion.h"
#include <stddef.h>
#include "global_uart_handler.h"


#include <stdio.h>

// volatile int8_t rx_data[BYTES];

typedef enum { MANUAL_MODE, TOUCHSCREEN_MODE, AUTOMATED_MODE } project_mode_t;

volatile project_mode_t current_mode =
    TOUCHSCREEN_MODE; // Start in touchscreen mode

int main(void) {
  // init for comms
  spi_main_init();
  write_mem_map();
  read_adc_id();
  global_uart_init();
  printf("Step 3: Setting up initial display...\n");
  // Main application loop
  global_uart_main_loop();
  // uart init
  nextion_init();

  while (1) {
    switch (current_mode) {
    case AUTOMATED_MODE:
      break;
    case MANUAL_MODE:
      break;
    default:
      puts("touchscreen mode \n\n");
      mode_touchscreen_run();
      break;
    }
  }
  MXC_SPI_Shutdown(SPI_MAIN);
  return E_NO_ERROR;
}

