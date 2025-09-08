#include "mode_manual.h"
#include "TMC5272.h"
#include "gpio.h"

void tricoder_handler(tmc5272_dev_t *tmc_x, tmc5272_dev_t *tmc_y, tmc5272_dev_t *tmc_tc) {

    printf("begin tricoder\n\n");

    // Read the Tricoder position
		int32_t tc_x_pos = tmc5272_tricoder_getPosition(tmc_tc, TC_X);
		int32_t tc_y_pos = tmc5272_tricoder_getPosition(tmc_tc, TC_Y);

		// Rotate each axis to its encoder position
		tmc5272_rotateToPosition(tmc_x, MOTOR_0, tc_x_pos);
		tmc5272_rotateToPosition(tmc_y, ALL_MOTORS, tc_y_pos);

		// Readout position & encoder
		printf("Mx0: %d  ENC: %d", tmc5272_getPosition(tmc_x, MOTOR_0), tc_x_pos);
		printf("\tMy0: %d  ENC: %d \n", tmc5272_getPosition(tmc_y, MOTOR_0), tc_y_pos);

		// // Failsafe brake
		// if(PB_IsPressedAny())
		// {
		// 	printf("Failsafe! \n");
		// 	tmc5272_rotateAtVelocity(&tmc_x, MOTOR_0, 0, 50000);
		// 	tmc5272_rotateAtVelocity(&tmc_x, MOTOR_1, 0, 50000);
		// 	tmc5272_rotateAtVelocity(&tmc_y, MOTOR_0, 0, 50000);
		// 	tmc5272_rotateAtVelocity(&tmc_y, MOTOR_1, 0, 50000);
			
		// 	while(1) {}
		// }
		

    // }
}