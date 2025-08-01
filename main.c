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
    uint32_t base1 = calibrate();
    MXC_Delay(MXC_DELAY_MSEC(1000));
    set_channel_2();
    uint32_t base2 = calibrate();
    MXC_Delay(MXC_DELAY_MSEC(1000));
    while(1) {
        set_channel_0();
        load_cell_1_read(base1);
        MXC_Delay(MXC_DELAY_MSEC(1000));
        set_channel_2();
        load_cell_2_read(base2);
        MXC_Delay(MXC_DELAY_MSEC(1000));
    }
    MXC_SPI_Shutdown(SPI_MAIN);
    return E_NO_ERROR;
}