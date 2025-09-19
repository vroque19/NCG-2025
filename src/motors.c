#include "motors.h"
#include "TMC5272.h"
#include "TMC5272_SPI.h"

tmc_devices_t tmc_devices = {};
void init_motors(void) {
    // Create GPIO Port/Pins Config struct
    static mxc_gpio_cfg_t spi_port_cfg;
    spi_port_cfg = TMC5272_SPI_PORT_CFG_MXC;
    spi_port_cfg.vssel = MXC_GPIO_VSSEL_VDDIOH;

    // Add masks for X, Y, and TC axes.
    spi_port_cfg.mask |= (TMC5272_SPI_PIN_X | TMC5272_SPI_PIN_Y | TMC5272_SPI_PIN_TC);
    // Allocate static memory for device structs
    static tmc5272_dev_t tmc_x_dev = {
        .spi_port = MXC_SPI1,
        .gpio_cfg_spi = &spi_port_cfg,
        .ss_index = TMC5272_SPI_CS_IDX_X
    };
    
    static tmc5272_dev_t tmc_y_dev = {
        .spi_port = MXC_SPI1,
        .gpio_cfg_spi = &spi_port_cfg,
        .ss_index = TMC5272_SPI_CS_IDX_Y
    };
    
    static tmc5272_dev_t tmc_tc_dev = {
        .spi_port = MXC_SPI1,
        .gpio_cfg_spi = &spi_port_cfg,
        .ss_index = TMC5272_SPI_CS_IDX_TC
    };
    
    // Assign pointers to the tmc_devices
    tmc_devices.tmc_x = &tmc_x_dev;
    tmc_devices.tmc_y = &tmc_y_dev;
    tmc_devices.tmc_tc = &tmc_tc_dev;
    
    // Initialize independent motors
    tmc5272_init(tmc_devices.tmc_x);
    tmc5272_init(tmc_devices.tmc_y);

    /**** Motor Setup ****/
    // Init tricoders
    tmc5272_tricoder_init(tmc_devices.tmc_tc, TC_X);
    tmc5272_tricoder_init(tmc_devices.tmc_tc, TC_Y);
    
    // Velocity
    tmc5272_setVelocityCurve(tmc_devices.tmc_x, MOTOR_0, TMC5272_VEL_MAX, TMC5272_ACC_MAX);
    tmc5272_setVelocityCurve(tmc_devices.tmc_y, ALL_MOTORS, TMC5272_VEL_MAX, TMC5272_ACC_MAX);
    tmc5272_setMotorDirection(tmc_devices.tmc_y, MOTOR_0, MOTOR_DIR_INVERT);

}

tmc5272_dev_t* get_tmc_x_device(void) {
    // Safety check to ensure init_motors() was called
    if (tmc_devices.tmc_x == NULL) {
        return NULL;
    }
    return tmc_devices.tmc_x;
}

tmc5272_dev_t* get_tmc_y_device(void) {
    if (tmc_devices.tmc_y == NULL) {
        return NULL;
    }
    return tmc_devices.tmc_y;
}

tmc5272_dev_t* get_tmc_tc_device(void) {
    if (tmc_devices.tmc_tc == NULL) {
        return NULL;
    }
    return tmc_devices.tmc_tc;
}