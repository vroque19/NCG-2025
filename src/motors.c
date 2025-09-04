#include "motors.h"

void init_motors(void) {
    
	// Create GPIO Port/Pins Config struct
    printf("Create GPIO Port\n\n");
	// Copy base MXC cfg struct. (MSDK uses const.)
    mxc_gpio_cfg_t spi_port_cfg = TMC5272_SPI_PORT_CFG_MXC;
	// Modify VSSEL (VDDIOH = 3.3V)
	spi_port_cfg.vssel = MXC_GPIO_VSSEL_VDDIOH;
	// Add masks for X, Y, and TC axes.
    spi_port_cfg.mask |= (TMC5272_SPI_SS_PIN_DEV_X | TMC5272_SPI_SS_PIN_DEV_Y | TMC5272_SPI_SS_PIN_DEV_TC);
    printf("Create Device Struct\n\n");
	
	// Create device struct for each IC
	tmc5272_dev_t* tmc_x = &(tmc5272_dev_t){
		.spi_port = MXC_SPI1,
		.gpio_cfg_spi = &spi_port_cfg,
		.ss_index = 1
	};
	tmc5272_dev_t* tmc_y = &(tmc5272_dev_t){
		.spi_port = MXC_SPI1,
		.gpio_cfg_spi = &spi_port_cfg,
		.ss_index = 2
	};
	tmc5272_dev_t* tmc_tc = &(tmc5272_dev_t){
		.spi_port = MXC_SPI1,
		.gpio_cfg_spi = &spi_port_cfg,
		.ss_index = 3
	};
    printf("Create Motor IC\n\n");

	// Initialize each moving motor
	tmc5272_init(tmc_x);
	tmc5272_init(tmc_y);
    printf("Create TRicoders\n\n");

	// Init tricoders
	tmc5272_tricoder_init(tmc_tc, TC_X);
	tmc5272_tricoder_init(tmc_tc, TC_Y);

    printf("Set Velocity Curve\n\n");
	
	/**** Motor Setup ****/
	// Velocity
	tmc5272_setVelocityCurve(tmc_x, MOTOR_0, 200000, 1000);
	tmc5272_setVelocityCurve(tmc_y, ALL_MOTORS, 100000, 5000);
	
	// Invert direction of necessary motors
	tmc5272_setMotorDirection(tmc_y, MOTOR_0, MOTOR_DIR_INVERT);

}