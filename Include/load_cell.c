#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "mxc_delay.h"
#include <string.h>
#include "4131.h"
#include "load_cell.h"

// converts ADC code to weight
// float code_to_grams(uint32_t code, uint32_t base) {
    
// }

float get_average(uint32_t *list, uint8_t n) {
    // get the length of the list
    uint32_t sum = 0;
    for(int i = 0; i < n; i++) {
        printf("%d: ", i);
        printf("%d\n", list[i]);
        // MXC_Delay(MXC_DELAY_MSEC(1000));
        sum += list[i];
    }
    float res = (float)sum / n;
    printf("resulting avg: %f", res);
    return res;
}

void test_get_average(void) {
    int n = 100;
    // uint32_t data[n];
    // memset(data, 0, sizeof(data));
    uint32_t *data = get_calibration_data();
    // get_average(data, n);
    // printf("%d", res);
    
}

uint32_t *get_calibration_data(void) {
    int n = 100;
    uint32_t data[n];
    for(int i = 0; i < n; i++) {
        data[i] = 50;
    }
    float res = get_average(data, n);
    printf("resulting avg: %f", res);
    return data;
}

uint32_t calibrate(void) {
    uint32_t base = 0;
    bool calibrated = false;

    return base;
    // }

}