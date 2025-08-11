#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <math.h>
#include "mxc_device.h"
#include "mxc_delay.h"
#include "mxc_pins.h"
#include "uart.h"
#include "led.h"
#include "spi.h"
#include "Include/4131.h"
#include "Include/load_cell.h"
#include "Include/nextion.h"


int main(void)
{
    spi_main_init();
    nextion_init();
    write_mem_map();
    read_adc_id();

    uint32_t base0 = calibrate(LOAD_CELL_0);
    MXC_Delay(MXC_DELAY_MSEC(1000));
    
    uint32_t base1 = calibrate(LOAD_CELL_1);
    MXC_Delay(MXC_DELAY_MSEC(1000));
    
    uint32_t base2 = calibrate(LOAD_CELL_2);
    while(1) {
        double weight0 = get_load_cell_data(LOAD_CELL_0, base0);
        double weight1 = get_load_cell_data(LOAD_CELL_1, base1);
        double weight2 = get_load_cell_data(LOAD_CELL_2, base2);
        MXC_Delay(MXC_DELAY_MSEC(100));
        update_weight(weight2, TXT_WEIGHT_0);
        MXC_Delay(MXC_DELAY_MSEC(100));
        update_weight(weight1, TXT_WEIGHT_1);
        MXC_Delay(MXC_DELAY_MSEC(100));
        update_weight(weight0, TXT_WEIGHT_2);

        
    }
    MXC_SPI_Shutdown(SPI_MAIN);
    return E_NO_ERROR;
}