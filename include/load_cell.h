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
extern weight_readings_t weight_readings;
uint32_t calibrate(uint8_t idx);
uint32_t get_calibration_data(void);
uint32_t get_average(uint32_t *list, uint8_t n);
double code_to_grams(uint32_t base, uint32_t code, double conversion_factor);
double get_load_cell_data(uint8_t channel_idx, uint32_t base);
void test_switch(uint32_t base0, uint32_t base1, uint32_t base2);

#endif