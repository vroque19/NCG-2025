/* 
MIT License

Copyright (c) 2025 Analog Devices

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "solenoid_driver.h"
#include <led.h>

mxc_gpio_cfg_t solenoid_gpio_out;
void solenoid_gpio_init(void) {
    solenoid_gpio_out.port = SOLENOID_GPIO_PORT_OUT;
    if(MXC_GPIO_Init(2) != E_NO_ERROR) {
        printf("GPIO error.\n"); while(1){}
    }
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
    printf("Successfully configured GPIO pin: %d\n\n", MXC_GPIO_OutGet(solenoid_gpio_out.port, SOLENOID_GPIO_PIN_OUT));
}

void solenoid_on(void) {
    MXC_GPIO_OutSet(SOLENOID_GPIO_PORT_OUT, SOLENOID_GPIO_PIN_OUT);
    // printf("Turning solenoid on. GPIO output state: %d\n\n", MXC_GPIO_OutGet(solenoid_gpio_out.port, SOLENOID_GPIO_PIN_OUT));


}
void solenoid_off(void) {
    MXC_GPIO_OutClr(solenoid_gpio_out.port, solenoid_gpio_out.mask);
    // printf("Turning solenoid off. GPIO output state: %d\n\n", MXC_GPIO_OutGet(solenoid_gpio_out.port, SOLENOID_GPIO_PIN_OUT));
}

void solenoid_toggle(void) {
    MXC_GPIO_OutToggle(solenoid_gpio_out.port, solenoid_gpio_out.mask);
    printf("Toggling GPIO. GPIO output state: %d\n\n", MXC_GPIO_OutGet(solenoid_gpio_out.port, SOLENOID_GPIO_PIN_OUT));
}