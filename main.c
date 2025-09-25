// #include "4131.h"
// #include "load_cell.h"
// #include "mxc_pins.h"
// #include "nextion.h"
// #include "mode_touchscreen.h"
// #include <stddef.h>
#include "4131.h"
#include "load_cell.h"
#include "motors.h"
#include "global_uart_handler.h"
#include "nextion.h"


int main(void) {
  // init for comms
  spi_main_init();
  write_mem_map();
  read_adc_id();

  
  // while(1) {
    // poll_weights();
    // configure_adc_channel(0, 0x08);
    // uint32_t data0 = get_adc_data();
    // printf("data 0: %d\n", data0);
    // configure_adc_channel(0, 0x0);
    // configure_adc_channel(1, 0x08);
    
    // uint32_t data1 = get_adc_data();
    // printf("data 0: %d\n", data1);

  // }
  // calibrate(1);
  // calibrate(2);
// init_motors();
  global_uart_init();
  solenoid_gpio_init();
  printf("Step 3: Setting up initial display...\n");
  // Main application loop
  global_uart_main_loop();
  MXC_SPI_Shutdown(SPI_MAIN);
  return E_NO_ERROR;
}