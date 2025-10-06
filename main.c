// #include "4131.h"
// #include "load_cell.h"
// #include "mxc_pins.h"
// #include "nextion.h"
// #include "mode_touchscreen.h"
// #include <stddef.h>
#include "load_cell.h"
#include "motors.h"
#include "global_uart_handler.h"
#include "nextion.h"


int main(void) {
  // init for comms
  spi_main_init();
  write_mem_map();
  read_adc_id();
// init_motors();
  global_uart_init();
  solenoid_gpio_init();
  printf("Setting up display...\n");
  // Main application loop
  global_uart_main_loop();
  free(load_cell_towers.load_cell_0);
  free(load_cell_towers.load_cell_1);
  free(load_cell_towers.load_cell_2);
  MXC_SPI_Shutdown(SPI_MAIN);
  return E_NO_ERROR;
}