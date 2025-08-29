// #include "4131.h"
// #include "load_cell.h"
// #include "mxc_pins.h"
// #include "nextion.h"
// #include "mode_touchscreen.h"
// #include <stddef.h>
#include "global_uart_handler.h"


int main(void) {
  // init for comms
  spi_main_init();
  write_mem_map();
  read_adc_id();
  global_uart_init();
  solenoid_gpio_init();
  printf("Step 3: Setting up initial display...\n");
  // Main application loop
  global_uart_main_loop();
  MXC_SPI_Shutdown(SPI_MAIN);
  return E_NO_ERROR;
}

