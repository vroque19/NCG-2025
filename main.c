#include "4131.h"
#include "load_cell.h"
#include "mxc_pins.h"
#include "nextion.h"
#include "mode_touchscreen.h"
#include <stddef.h>


typedef enum {
    MANUAL_MODE,
    TOUCHSCREEN_MODE,
    AUTOMATED_MODE
} project_mode_t;

volatile project_mode_t current_mode = TOUCHSCREEN_MODE; // Start in touchscreen mode

int main(void) {
  spi_main_init();
  write_mem_map();
  read_adc_id();
  // uart init
  nextion_init();

  
  while(1) {
	switch (current_mode) {
	case AUTOMATED_MODE:
		break;
	case MANUAL_MODE:
		break;
	default:
	printf("touchscreen mode \n\n");
		mode_touchscreen_run();
		break;
	}

    }
  MXC_SPI_Shutdown(SPI_MAIN);
  return E_NO_ERROR;
}
