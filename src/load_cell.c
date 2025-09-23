#include "load_cell.h"
#include "4131.h"
#include "mxc_delay.h"
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// calibrate the scale
// converts ADC code to weight

uint32_t get_average(uint32_t *list, uint8_t n) {
  uint32_t sum = 0;
  for (int i = 0; i < n; i++) {
    sum += list[i];
  }
  uint32_t res = floor(sum / n);
  printf("Get avg: %d\n", res);

  return res;
}

uint32_t get_calibration_data(void) {
  int n = 4;
  uint32_t data[n];
  MXC_Delay(MXC_DELAY_MSEC(200));
  for (int i = 0; i <= n; i++) {
    data[i] = get_adc_data();
    printf("\n- - - - - -%d- - - - - -\n", data[i]);
    MXC_Delay(MXC_DELAY_MSEC(250));
  }
  data[0] = data[1];
  uint32_t res = get_average(data, n);
  return res;
}

uint32_t calibrate(uint8_t idx) {
  configure_adc_channel(idx, 0x80); // enable
  printf("~~Taring scale %d~~\n", idx);

  uint32_t base = get_calibration_data();
  printf("Return from cal\n");

  printf("~~Calibration %d Complete~~\n", idx);
  configure_adc_channel(idx, 0x00); // disable
  return base;
}

double code_to_grams(uint32_t base, uint32_t code, double conversion_factor) {
  // printf("Code - Reference : %d - %d\n", code, base);
  double delta = abs((int)code - (int)base);
  if (delta > 1) {
    double grams = delta * conversion_factor;
    return grams;
  }
  return 0;
}

double get_load_cell_data(uint8_t channel_idx, uint32_t base) {
  // enable channel
  configure_adc_channel(channel_idx, 0x80);
  // delay 250 ms then read data
  MXC_Delay(MXC_DELAY_MSEC(250));
  uint32_t code = get_adc_data();
  // check the status
  // read_status();
  double conversion_factors[] = {0.41, 0.39, 0.39};
  double weight = code_to_grams(base, code, conversion_factors[channel_idx]);
  printf("***********\n");
  printf("Load cell %d weight in grams: %.2fg\n", channel_idx, weight);
  printf("***********\n");
  // disable channel
  configure_adc_channel(channel_idx, 0x00);
  printf("\n");
  return weight;
}
