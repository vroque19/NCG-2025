#ifndef _load_cell_h_
#define _load_cell_h_

uint32_t calibrate(void);
void test_get_average(void);
uint32_t get_calibration_data(void);
uint32_t get_average(uint32_t *list, uint8_t n);
void load_cell_1_read(uint32_t base);
void load_cell_2_read(uint32_t base);
double get_weight(uint32_t base);
double code_to_grams(uint32_t base, uint32_t code, double conversion_factor);
#endif