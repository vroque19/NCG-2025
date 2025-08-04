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
    return res;
}


uint32_t get_calibration_data(void) {
    int n = 50;
    uint32_t data[n];
    for(int i = 0; i < n; i++) {
        data[i] = get_adc_data();
        MXC_Delay(MXC_DELAY_MSEC(10));

    }
    uint32_t res = get_average(data, n);
    // printf("resulting avg: %d\n\n", res);

    return res;
}

uint32_t calibrate_0(void) {
    printf("~~Taring scale 0~~\n");
    uint32_t base = get_calibration_data();
    printf("~~Calibration Complete~~\n");
    return base;
}

uint32_t calibrate_1(void) {
    printf("~~Taring scale 1~~\n");
    uint32_t base = get_calibration_data();
    printf("~~Calibration Complete~~\n");
    return base;
}

double code_to_grams(uint32_t base, uint32_t code, double conversion_factor){
    printf("Code - Reference :%d - %d\n", code, base);
    double delta = abs((int)code - (int)base);
    if(delta > 1) {
        double grams = delta * conversion_factor;
        return grams;
    }
    return 0;
}


// void load_cell_0_read(uint32_t base) {
//     // printf("0.00~=code: %d\n\n", base);
//     // set_channel_0();
//     // uint32_t code = get_data_from_channel(0);
//     uint32_t code = get_adc_data();
//     double conversion_factor = 0.766561084;
//     double weight = code_to_grams(base, code, conversion_factor);
//     // FILE *file;
//     // file = fopen("load0_1.txt", "w");
//     // if(file == NULL) {
//     //     return;
//     // }
//     // fprintf(file, "Enabling both channels and letting the handler poll both channels\n");
//     // fprintf(file, "Expected 0g - Got %dg\n", weight);
//     printf("Load cell 1 Weight in grams: %.2fg\n", weight);

// }

// void load_cell_1_read(uint32_t base) {
//     // set_channel_1();
//     uint32_t code = get_adc_data();
//     double conversion_factor = 0.7532382584;
//     double weight = code_to_grams(base, code, conversion_factor);
//     // FILE *file;
//     // file = fopen("load1_1.txt", "w");
//     // if(file == NULL) {
//     //     return;
//     // }
//     // fprintf(file, "Enabling both channels and letting the handler poll both channels\n");
//     // fprintf(file, "Expected 0g - Got %dg\n", weight);
//     printf("Load cell 2 Weight in grams: %.2fg\n\n", weight);


// }

void get_load_cell_data(uint8_t channel_idx, uint32_t base) {
    // channel configuration
    configure_adc_channel(channel_idx, 0x80);
    // check the status
    printf("Waiting for data ready on channel %d...\n", channel_idx);
    uint8_t rdy = read_status();
    // while((rdy & 0x10) != 0) {
    //     printf("Waiting for the ready bit %d to flip", rdy);
    //     MXC_Delay(MXC_DELAY_MSEC(500));
    // }
    // printf("Ready!!!");
    // get data
    MXC_Delay(MXC_DELAY_MSEC(1500));
    uint32_t code = get_adc_data();
    read_status();
    double conversion_factor = 0.7532382584;
    double weight = code_to_grams(base, code, conversion_factor);
    // disable channel
    printf("Load cell %d Weight in grams: %.2fg\n\n", channel_idx, weight);
    configure_adc_channel(channel_idx, 0x00);
    // read_status();


}