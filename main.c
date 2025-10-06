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
  configure_adc_channel(0, 0x00); // disable
  configure_adc_channel(1, 0x00); // disable
  configure_adc_channel(2, 0x00); // disable
  configure_adc_channel(3, 0x00); // disable

  configure_adc_channel(0, 0x80); // enable
  get_data();
  while(1) {
    uint32_t data = get_adc_data();
    printf("Code for LC 0: %d\n", data);
    MXC_Delay(200000);

  }
  // Close the file

  MXC_SPI_Shutdown(SPI_MAIN);
  return E_NO_ERROR;
}
