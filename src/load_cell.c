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

#include "load_cell.h"
#include "4131.h"
#include <stdint.h>

load_cell_towers_t load_cell_towers = {0};

uint16_t get_average(uint16_t *list, uint8_t n) {
  uint16_t sum = 0;
  for (int i = 0; i < n; i++) {
    sum += list[i];
  }
  uint16_t res = floor(sum / n);
  return res;
}

/* samples load cell and returns the average reading */
uint16_t get_calibration_data(void) {
  int n = 5;
  uint16_t data[n];
  MXC_Delay(MXC_DELAY_MSEC(250));
  for (int i = 0; i < n; i++) {
    data[i] = get_adc_data();
    MXC_Delay(MXC_DELAY_MSEC(10));
  }
  uint16_t res = get_adc_data();
  return res;
}

/* calibrates load cell */
uint16_t calibrate(uint8_t idx) {
  configure_adc_channel(idx, 0x80); // enable
  printf("~~Taring scale %d~~\n", idx);
  uint16_t base = get_calibration_data();
  configure_adc_channel(idx, 0x00); // disable
  printf("~~Calibration %d Complete~~\n", idx);
  return base;
}

void calibrate_towers(void) {
  // Correctly allocate memory for each load_cell_t struct on the heap
  load_cell_towers.load_cell_0 = (load_cell_t *)malloc(sizeof(load_cell_t));
  load_cell_towers.load_cell_1 = (load_cell_t *)malloc(sizeof(load_cell_t));
  load_cell_towers.load_cell_2 = (load_cell_t *)malloc(sizeof(load_cell_t));
  uint16_t calibration_data_0 = calibrate(LOAD_CELL_0);
  printf("LC 0: %d\n", calibration_data_0);
  load_cell_towers.load_cell_0->base_offset = calibration_data_0;
  load_cell_towers.load_cell_0->conversion_factor = 0.3920992819;

  uint16_t calibration_data_1 = calibrate(LOAD_CELL_1);
  printf("LC 1: %d\n", calibration_data_1);
  load_cell_towers.load_cell_1->base_offset = calibration_data_1;
  load_cell_towers.load_cell_1->conversion_factor = 0.3920992819;

  uint16_t calibration_data_2 = calibrate(LOAD_CELL_2);
  printf("LC 2: %d\n", calibration_data_2);
  load_cell_towers.load_cell_2->base_offset = calibration_data_2;
  load_cell_towers.load_cell_2->conversion_factor = 0.3920992819;
  return;
}

/* converts adc code to grams */
double code_to_grams(uint16_t base, uint16_t code, double conversion_factor) {
  printf("Code - Reference : %d - %d\n", code, base);
  double delta = abs((int)code - (int)base);
  if (delta > 1) {
    double grams = delta * conversion_factor;
    return grams;
  }
  return 0;
}

// returns current reading from selected load cell
double get_load_cell_data(uint8_t channel_idx, uint16_t base) {
  // enable channel
  configure_adc_channel(channel_idx, 0x80);
  // wait before data read to allow the register to populate
  MXC_Delay(MXC_DELAY_MSEC(400));
  uint16_t code = get_adc_data();
  // if(code - base < 30) {
  //     code = base;
  // }
  // check the status
  // read_status();

  double weight = code_to_grams(base, code, .39);
  // disable channel
  configure_adc_channel(channel_idx, 0x00);
  return weight;
}
