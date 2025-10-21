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