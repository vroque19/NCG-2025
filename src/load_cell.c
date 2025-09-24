#include "load_cell.h"
#include "4131.h"


load_cell_towers_t load_cell_towers = {0};
uint32_t get_average(uint32_t *list, uint8_t n) {
    uint32_t sum = 0;
    for(int i = 0; i < n; i++) {
        sum += list[i];
    }
    uint32_t res = floor(sum / n);
    return res;
}

/* samples load cell and returns the average reading */
uint32_t get_calibration_data(void) {
    int n = 10;
    uint32_t data[n];
    MXC_Delay(MXC_DELAY_MSEC(250));
    for(int i = 0; i < n; i++) {
        data[i] = get_adc_data();
        MXC_Delay(MXC_DELAY_MSEC(10));
    }
    uint32_t res = get_adc_data();
    return res;
}

/* calibrates load cell */
uint32_t calibrate(uint8_t idx) {
    configure_adc_channel(idx, 0x80); // enable
    printf("~~Taring scale %d~~\n", idx);
    uint32_t base = get_calibration_data();
    configure_adc_channel(idx, 0x00); // disable
    printf("~~Calibration %d Complete~~\n", idx);
    return base;
}

void calibrate_towers(void) {
   // Correctly allocate memory for each load_cell_t struct on the heap
  load_cell_towers.load_cell_0 = (load_cell_t*)malloc(sizeof(load_cell_t));
  load_cell_towers.load_cell_1 = (load_cell_t*)malloc(sizeof(load_cell_t));
  load_cell_towers.load_cell_2 = (load_cell_t*)malloc(sizeof(load_cell_t));
  uint32_t calibration_data_0 = calibrate(0);
    printf("LC 0: %d\n", calibration_data_0);
    load_cell_towers.load_cell_0->base_offset = calibration_data_0;
    load_cell_towers.load_cell_0->conversion_factor = 0.3920992819;

    uint32_t calibration_data_1 = calibrate(1);
    printf("LC 1: %d\n", calibration_data_1);
    load_cell_towers.load_cell_1->base_offset = calibration_data_1;
    load_cell_towers.load_cell_1->conversion_factor = 0.3920992819;

    uint32_t calibration_data_2 = calibrate(2);
    printf("LC 2: %d\n", calibration_data_2);
    load_cell_towers.load_cell_2->base_offset = calibration_data_2;
    load_cell_towers.load_cell_2->conversion_factor = 0.3920992819;
}

/* converts adc code to grams */
double code_to_grams(uint32_t base, uint32_t code, double conversion_factor){
    printf("Code - Reference : %d - %d\n", code, base);
    double delta = abs((int)code - (int)base);
    if(delta > 1) {
        double grams = delta * conversion_factor;
        return grams;
    }
    return 0;
}

// returns current reading from selected load cell
double get_load_cell_data(uint8_t channel_idx, uint32_t base) {
    // enable channel
    configure_adc_channel(channel_idx, 0x80);
    // wait before data read to let the register populate
    MXC_Delay(MXC_DELAY_MSEC(250));
    uint32_t code = get_adc_data();
    // check the status
    // read_status();
  
    double weight = code_to_grams(base, code, .39);
    // disable channel
    configure_adc_channel(channel_idx, 0x00);
    return weight;
}
