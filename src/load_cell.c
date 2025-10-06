#include "load_cell.h"
#include "4131.h"
#include "mxc_delay.h"


load_cell_towers_t load_cell_towers = {0};
uint16_t get_average(uint16_t *list, uint8_t n) {
    uint16_t sum = 0;
    for(int i = 0; i < n; i++) {
        sum += list[i];
    }
    uint16_t res = floor(sum / n);
    printf("AVG: %d\n", res);

    return res;
}

/* samples load cell and returns the average reading */
uint16_t get_data(void) {
    int n = 20;
    uint16_t data[n];
    MXC_Delay(MXC_DELAY_MSEC(250));
    for(int i = 0; i < n; i++) {
        data[i] = get_adc_data();
        printf("%d\n", data[i]);
        MXC_Delay(MXC_DELAY_MSEC(5));
    }
    uint16_t res = get_average(data, n-1);
    printf("AVG: %d\n", res);

    return res;
}

/* calibrates load cell */
uint16_t calibrate(uint8_t idx) {
    configure_adc_channel(idx, 0x80); // enable
    printf("~~Taring scale %d~~\n", idx);
    uint16_t base = get_data();
    configure_adc_channel(idx, 0x00); // disable
    printf("~~Calibration %d Complete~~\n", idx);
    return base;
}

void poll_adc(void) {
    for(int i = 0; i < 4; i++) {
        configure_adc_channel(i, 0x80);
        uint16_t adc_output = get_adc_data();
        printf("LC %d: %d\n", i, adc_output);
        configure_adc_channel(i, 0x00); // disable
        MXC_Delay(MXC_DELAY_MSEC(200));
    }
    printf("\n\n");
}

void calibrate_towers(void) {
   // Correctly allocate memory for each load_cell_t struct on the heap
  load_cell_towers.load_cell_0 = (load_cell_t*)malloc(sizeof(load_cell_t));
  load_cell_towers.load_cell_1 = (load_cell_t*)malloc(sizeof(load_cell_t));
  load_cell_towers.load_cell_2 = (load_cell_t*)malloc(sizeof(load_cell_t));
  uint16_t calibration_data_0 = calibrate(0);
    printf("LC 0: %d\n", calibration_data_0);
    load_cell_towers.load_cell_0->base_offset = calibration_data_0;
    load_cell_towers.load_cell_0->conversion_factor = 0.3920992819;

    uint16_t calibration_data_1 = calibrate(1);
    printf("LC 1: %d\n", calibration_data_1);
    load_cell_towers.load_cell_1->base_offset = calibration_data_1;
    load_cell_towers.load_cell_1->conversion_factor = 0.3920992819;

    uint16_t calibration_data_2 = calibrate(2);
    printf("LC 2: %d\n", calibration_data_2);
    load_cell_towers.load_cell_2->base_offset = calibration_data_2;
    load_cell_towers.load_cell_2->conversion_factor = 0.3920992819;
}

/* converts adc code to grams */
double code_to_grams(uint16_t base, uint16_t code, double conversion_factor){
    printf("Code - Reference : %d - %d\n", code, base);
    double delta = abs((int)code - (int)base);
    if(delta > 1) {
        double grams = delta * conversion_factor;
        return grams;
    }
    return 0;
}

// returns current reading from selected load cell
double get_load_cell_data(uint8_t channel_idx, uint16_t base) {
    // enable channel
    configure_adc_channel(channel_idx, 0x80);
    MXC_Delay(MXC_DELAY_MSEC(100));
    uint16_t code = get_adc_data();
    printf("COde: %d\n\n", code);
    double weight = code_to_grams(base, code, .39);
    // disable channel
    configure_adc_channel(channel_idx, 0x00);
    return weight;
}
