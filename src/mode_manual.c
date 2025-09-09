#include "mode_manual.h"
#include "TMC5272.h"
#include "game_logic.h"
#include "global_uart_handler.h"

void tricoder_handler(tmc5272_dev_t *tmc_x, tmc5272_dev_t *tmc_y, tmc5272_dev_t *tmc_tc) {

    printf("begin tricoder\n\n");
		while(current_mode!=MENU) {

			// Read the Tricoder position
				int32_t tc_x_pos = tmc5272_tricoder_getPosition(tmc_tc, TC_X);
				int32_t tc_y_pos = tmc5272_tricoder_getPosition(tmc_tc, TC_Y);
		
				// Rotate each axis to its encoder position
				tmc5272_rotateToPosition(tmc_x, MOTOR_0, tc_x_pos);
				tmc5272_rotateToPosition(tmc_y, ALL_MOTORS, tc_y_pos);
		} return;

}