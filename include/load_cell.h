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

#ifndef _load_cell_h_
#define _load_cell_h_
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "4131.h"

#define LOAD_CELL_0 0
#define LOAD_CELL_1 1
#define LOAD_CELL_2 2

// struct to hold current polled readings
typedef struct {
    double weights[3];
} weight_readings_t;

typedef struct {
    double base_offset;
    double conversion_factor;
} load_cell_t;

typedef struct {
    load_cell_t* load_cell_0;
    load_cell_t* load_cell_1;
    load_cell_t* load_cell_2;
} load_cell_towers_t;

extern load_cell_towers_t load_cell_towers;
extern weight_readings_t weight_readings;

uint16_t calibrate(uint8_t idx);
uint16_t get_calibration_data(void);
void calibrate_towers(void);
uint16_t get_average(uint16_t *list, uint8_t n);
double code_to_grams(uint16_t base, uint16_t code, double conversion_factor);
double get_load_cell_data(uint8_t channel_idx, uint16_t base);
void test_switch(uint16_t base0, uint16_t base1, uint16_t base2);

#endif