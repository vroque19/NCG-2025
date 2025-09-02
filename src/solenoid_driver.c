#include "solenoid_driver.h"

mxc_gpio_cfg_t solenoid_gpio_out;
void solenoid_gpio_init(void) {
    solenoid_gpio_out.port = SOLENOID_GPIO_PORT_OUT;
    // if(MXC_GPIO_Init(2) != E_NO_ERROR) {
    //     printf("GPIO error.\n"); while(1){}
    // }
    solenoid_gpio_out.mask = SOLENOID_GPIO_PIN_OUT;
    solenoid_gpio_out.drvstr = MXC_GPIO_DRVSTR_0;
    solenoid_gpio_out.func = MXC_GPIO_FUNC_OUT;
    solenoid_gpio_out.pad = MXC_GPIO_PAD_NONE;
    solenoid_gpio_out.vssel = MXC_GPIO_VSSEL_VDDIOH;
    if(MXC_GPIO_Config(&solenoid_gpio_out) != E_NO_ERROR) {
        printf("Error configuring solenoid gpio\n\n");
    }
    MXC_GPIO_Config(&solenoid_gpio_out);
    MXC_GPIO_OutClr(solenoid_gpio_out.port, solenoid_gpio_out.mask);
    printf("Successfully configured GPIO pin\n\n%d\n\n", MXC_GPIO_OutGet(solenoid_gpio_out.port, SOLENOID_GPIO_PIN_OUT));
}

void solenoid_on(void) {
    MXC_GPIO_OutSet(SOLENOID_GPIO_PORT_OUT, SOLENOID_GPIO_PIN_OUT);
    printf("Turning solenoid on. GPIO output state: %d\n\n", MXC_GPIO_OutGet(solenoid_gpio_out.port, SOLENOID_GPIO_PIN_OUT));
}
void solenoid_off(void) {
    MXC_GPIO_OutClr(solenoid_gpio_out.port, solenoid_gpio_out.mask);
    printf("Turning solenoid off. GPIO output state: %d\n\n", MXC_GPIO_OutGet(solenoid_gpio_out.port, SOLENOID_GPIO_PIN_OUT));

    // MXC_GPIO_OutToggle(SOLENOID_GPIO_PORT_OUT, SOLENOID_GPIO_PIN_OUT);
}

void solenoid_toggle(void) {
    MXC_GPIO_OutToggle(solenoid_gpio_out.port, solenoid_gpio_out.mask);
    printf("Toggling GPIO. GPIO output state: %d\n\n", MXC_GPIO_OutGet(solenoid_gpio_out.port, SOLENOID_GPIO_PIN_OUT));
}