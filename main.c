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
#include "4131.h"
#include "load_cell.h"


int main(void)
{
    spi_main_init();
    write_mem_map();
    read_adc_id();

    uint32_t base0 = calibrate(0);
    MXC_Delay(MXC_DELAY_MSEC(1500));
    
    uint32_t base1 = calibrate(1);
    MXC_Delay(MXC_DELAY_MSEC(1500));
    
    uint32_t base2 = calibrate(2);
    while(1) {
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
        // get_load_cell_data(2, base2);
        MXC_Delay(MXC_DELAY_MSEC(500));
    }
    MXC_SPI_Shutdown(SPI_MAIN);
    return E_NO_ERROR;
}