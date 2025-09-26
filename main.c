#include "4131.h"
#include "led.h"
#include "load_cell.h"
#include "mxc_delay.h"
#include "mxc_device.h"
#include "mxc_pins.h"
#include "spi.h"
#include "uart.h"
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int main(void) {
  spi_main_init();
  write_mem_map();
  read_adc_id();
  // configure_adc_channel(1, 0x00); // disable
  // configure_adc_channel(2, 0x00); // disable

  // configure_adc_channel(0, 0x80); // enable
  // get_adc_data();
  // // base 0: 0.3920992819
  // uint32_t base0 = calibrate(0);
  // MXC_Delay(MXC_DELAY_MSEC(1500));
  calibrate_towers();
  // uint32_t base1 = calibrate(1);
  // MXC_Delay(MXC_DELAY_MSEC(1500));
  // uint32_t base2 = calibrate(2);
  // uint32_t data = get_calibration_data();
  // printf("\n- - - - - -\nAverage 1: %d\n- - - - - -\n\n", data);
  //uint32_t data2 = get_calibration_data();
  //printf("\n- - - - - -\nAverage 2: %d\n- - - - - -\n\n", data2);
  while (1) {
    get_load_cell_data(1, 800);
    MXC_Delay(MXC_DELAY_MSEC(100));

  }
  MXC_SPI_Shutdown(SPI_MAIN);
  return E_NO_ERROR;
}
