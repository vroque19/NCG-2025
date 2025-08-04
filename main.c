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
#include "include/4131.h"
#include "include/load_cell.h"


int main(void)
{
    spi_main_init();
    write_mem_map();
    read_adc_id();
    read_adc_conversion();
    // set_channel_1();
    // set_channel_1();
    // set_channel_1();
    // set_channel_0();
    // set_channel_1();
    
    // set_channel_0();
    // set_channel_1();
    // set_channel_0();
    uint32_t base0 = calibrate(0);
    MXC_Delay(MXC_DELAY_MSEC(1500));

    set_channel_1();
    // read_status();
    // set_channel_1();
    uint32_t base1 = calibrate(1);
    // read_status();

    while(1) {
        // get_load_cell_data(0, base0);
        // get_data_from_channel(0);
        // MXC_Delay(MXC_DELAY_MSEC(5));
        get_load_cell_data(0, base0);
        // load_cell_1_read(base1);
        MXC_Delay(MXC_DELAY_MSEC(500));
        get_load_cell_data(1, base1);
        MXC_Delay(MXC_DELAY_MSEC(500));
    }
    MXC_SPI_Shutdown(SPI_MAIN);
    return E_NO_ERROR;
}