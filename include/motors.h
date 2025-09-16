#ifndef MOTORS_H
#define MOTORS_H

#include "TMC5272.h"
#include "TMC5272_SPI.h"
// /**** Macros ****/

// // Map tricoder inputs for X and Y axes
#define TC_X		MOTOR_0
#define TC_Y		MOTOR_1

// // TMC5272 Comms
#define TMC5272_SPI_PORT			MXC_SPI1
#define TMC5272_SPI_PORT_CFG_MXC	gpio_cfg_spi1

// // SS Pin & Index
#define TMC5272_SPI_PIN_X	MXC_GPIO_PIN_25
#define TMC5272_SPI_CS_IDX_X	1
#define TMC5272_SPI_PIN_Y	MXC_GPIO_PIN_24
#define TMC5272_SPI_CS_IDX_Y	2
#define TMC5272_SPI_PIN_TC	MXC_GPIO_PIN_27
#define TMC5272_SPI_CS_IDX_TC	3

// Velocity and Acceleration Curve
#define TMC5272_VEL_MAX 300000
#define TMC5272_ACC_MAX 10000

// Holds all 3 tmc devices
typedef struct {
    tmc5272_dev_t* tmc_x;
    tmc5272_dev_t* tmc_y;
    tmc5272_dev_t* tmc_tc;
} tmc_devices_t;
extern tmc_devices_t tmc_devices;
// Function to get the initialized device structs
tmc5272_dev_t* get_tmc_x_device(void);
tmc5272_dev_t* get_tmc_y_device(void);
tmc5272_dev_t* get_tmc_tc_device(void);
void init_motors(void);

#endif