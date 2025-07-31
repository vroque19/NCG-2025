#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include "mxc_delay.h"
#include <string.h>
#include "4131.h"
#include "load_cell.h"

// converts ADC code to weight

double get_average(uint32_t *list, uint8_t n) {
    // get the length of the list
    uint32_t sum = 0;
    for(int i = 0; i < n; i++) {
        printf("%d: ", i);
        printf("%d\n", list[i]);
        sum += list[i];
    }
    double res = sum / n;
    printf("\n\n");
    // printf("resulting avg: %f", res);
    return res;
}


void get_calibration_data(void) {
    int n = 3;
    uint32_t data[n];
    for(int i = 0; i < n; i++) {
        data[i] = get_adc_data();
        printf(">>>> code: %d\n", data[i]);
        MXC_Delay(MXC_DELAY_MSEC(10));


    }
    printf("~~Calibrated~~\n");
    double res = get_average(data, n);
    printf("resulting avg: %.5f\n", res);
}


