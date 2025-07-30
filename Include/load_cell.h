#ifndef _load_cell_h_
#define _load_cell_h_

float code_to_grams(uint32_t code, uint32_t base); 
uint32_t calibrate(void);
void test_get_average(void);
uint32_t *get_calibration_data(void);
float get_average(uint32_t *list, uint8_t n);

#endif