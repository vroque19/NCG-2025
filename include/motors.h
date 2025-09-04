#include "TMC5272.h"
/**** Macros ****/

// Map tricoder inputs for X and Y axes
#define TC_X		MOTOR_0
#define TC_Y		MOTOR_1

// TMC5272 Comms
#define TMC5272_SPI_PORT			MXC_SPI1
#define TMC5272_SPI_PORT_CFG_MXC	gpio_cfg_spi1

// SS Pin & Index
#define TMC5272_SPI_SS_PIN_DEV_X	MXC_GPIO_PIN_25
#define TMC5272_SPI_SS_IDX_DEV_X	1
#define TMC5272_SPI_SS_PIN_DEV_Y	MXC_GPIO_PIN_24
#define TMC5272_SPI_SS_IDX_DEV_Y	2
#define TMC5272_SPI_SS_PIN_DEV_TC	MXC_GPIO_PIN_27
#define TMC5272_SPI_SS_IDX_DEV_TC	3

// extern volatile tmc5272_dev_t tmc_x;
// extern volatile tmc5272_dev_t tmc_y;
// extern volatile tmc5272_dev_t tmc_tc;
/**** Initialize TMC5272 Devices ******/
void init_motors(void);