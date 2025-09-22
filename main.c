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
  // get_adc_data();
  // // base 0: 0.3920992819
  // uint32_t base0 = calibrate(0);
  // MXC_Delay(MXC_DELAY_MSEC(1500));

  // uint32_t base1 = calibrate(1);
  // MXC_Delay(MXC_DELAY_MSEC(1500));

  // uint32_t base2 = calibrate(2);
  get_adc_data();
  // configure_adc_channel(1, 0x00); // disable
  // configure_adc_channel(2, 0x00); // disable
  // configure_adc_channel(0, 0x80);
  // get_adc_data();

  // uint32_t data_1 = get_calibration_data();
  // printf("\n- - - - - -\nAverage 0.1: %d\n- - - - - -\n\n", data_1);
  // uint32_t data2_1 = get_calibration_data();
  // printf("\n- - - - - -\nAverage 0.2: %d\n- - - - - -\n\n", data2_1);
  // configure_adc_channel(0, 0x00); // disable
  // configure_adc_channel(2, 0x00); // disable
  // configure_adc_channel(1, 0x80);
  // get_adc_data();
  // uint32_t data_0 = get_calibration_data();
  // printf("\n- - - - - -\nAverage 1.1: %d\n- - - - - -\n\n", data_0);
  // uint32_t data2_0 = get_calibration_data();
  // printf("\n- - - - - -\nAverage 1.2: %d\n- - - - - -\n\n", data2_0);

  configure_adc_channel(1, 0x00); // disable
  configure_adc_channel(0, 0x00); // disable
  configure_adc_channel(2, 0x80);
  // uint32_t data_2 = get_calibration_data();
  get_adc_data();
  // printf("\n- - - - - -\nAverage 2.1: %d\n- - - - - -\n\n", data_2);
  // uint32_t data2_2 = get_calibration_data();
  // printf("\n- - - - - -\nAverage 2.2: %d\n- - - - - -\n\n", data2_2);
  while (1) {
    uint32_t data_2 = get_calibration_data();

    // printf("Select a load cell 0-2:\n ");
    // uint8_t cell_idx;
    // scanf("%d", &cell_idx);
    // printf("%d\n", cell_idx);

    // switch (cell_idx) {
    // case 0:
    //   get_load_cell_data(cell_idx, base0);
    //   break;
    // case 1:
    //   get_load_cell_data(cell_idx, base1);
    //   break;
    // default:
    //   get_load_cell_data(cell_idx, base2);
    //   break;
    // }
    // get_load_cell_data(2, base2);
    // MXC_Delay(MXC_DELAY_MSEC(900));
  }
  MXC_SPI_Shutdown(SPI_MAIN);
  return E_NO_ERROR;
}
