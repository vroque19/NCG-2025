#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "mxc_delay.h"
#include <string.h>
#include "4131.h"
#include "load_cell.h"

// calibrate the scale
// converts ADC code to weight

uint32_t get_average(uint32_t *list, uint8_t n) {
    uint32_t sum = 0;
    for(int i = 0; i < n; i++) {
        sum += list[i];
    }
    uint32_t res = floor(sum / n);
    printf("\n\n");
    return res;
}


uint32_t get_calibration_data(void) {
    int n = 3;
    uint32_t data[n];
    for(int i = 0; i < n; i++) {
        data[i] = get_adc_data();
        // printf(">>>> code: %d\n", data[i]);
        MXC_Delay(MXC_DELAY_MSEC(10));

    }
    uint32_t res = get_average(data, n);
    // printf("resulting avg: %d\n\n", res);
    return res;
}

uint32_t calibrate(void) {
    printf("~~Taring the scale~~\n");
    uint32_t base = get_calibration_data();
    printf("0.00~=code: %d\n\n", base);
    return base;
}

double code_to_grams(uint32_t base, uint32_t code, double conversion_factor){
    printf("%d - %d\n", code, base);
    double delta = abs((int)code - (int)base);
    if(delta > 2) {
        double grams = delta * conversion_factor;
        return grams;
    }
    // printf("Weight in grams: %dg\n\n", grams);
    return 0;
}


void load_cell_1_read(uint32_t base) {
    printf("0.00~=code: %d\n\n", base);
    uint32_t code = get_adc_data();
    double conversion_factor = 0.766561084;
    double weight = code_to_grams(base, code, conversion_factor);

    printf("Weight in grams: %.2fg\n\n", weight);

}
void load_cell_2_read(uint32_t base) {
    printf("0.00~=code: %d\n\n", base);
    uint32_t code = get_adc_data();
    double conversion_factor = 0.7532382584;
    double weight = code_to_grams(base, code, conversion_factor);
    printf("Weight in grams: %.2fg\n\n", weight);


}


