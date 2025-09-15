#include "load_cell.h"
#include "4131.h"

 // conversion factors for 3 load cells
 int global_base_array[3] = {32749, 32787, 31948};
//  int global_base_array[3] = {33063, 33509, 32755};

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

double get_load_cell_data(uint8_t channel_idx, uint32_t base) {
    // enable channel
    configure_adc_channel(channel_idx, 0x80);
    // wait before data read
    MXC_Delay(MXC_DELAY_MSEC(300));
    uint32_t code = get_adc_data();
    // printf("%d - %d\n", code, base);

    // check the status
    // read_status();
    double conversion_factors[] = {0.766561084, 0.77, 0.76};
    double weight = code_to_grams(base, code, conversion_factors[channel_idx]);
    // printf("***********\n");
    printf("Load cell %d weight in grams: %.2fg\n", channel_idx, weight);
    // printf("***********\n");
    // printf("\n");
    // disable channel
    configure_adc_channel(channel_idx, 0x00);
    return weight;
}

/* for testing */
void test_switch(uint32_t base0, uint32_t base1, uint32_t base2) {
    // switch statement load cell test
    printf("Select a load cell 0-2:\n ");
        uint8_t cell_idx;
        scanf("%d", &cell_idx);
        printf("%d\n", cell_idx);
        
        switch(cell_idx) {
            case 0:
                get_load_cell_data(cell_idx, base0);
                break;
            case 1:
                get_load_cell_data(cell_idx, base1);
                break;
            default:
                get_load_cell_data(cell_idx, base2);
                break;
        }
        MXC_Delay(MXC_DELAY_MSEC(500));
}