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

#include "nextion.h"
#include "game_logic.h"
#include "load_cell.h"
#include "mxc_delay.h"
#include "uart.h"


volatile uint32_t *uart_int_en, *uart_int_flags; 
weight_readings_t weight_readings = {};
/* Initialize UART for the Nextion display */
void nextion_init(void) {
    if(MXC_UART_Init(GLOBAL_UART_REG, BAUD_RATE) != E_NO_ERROR) {
        printf("\n UART INIT ERROR.\n");
        while(1){}
    }
    // Set GPIO for UART
    const mxc_gpio_cfg_t uart_gpio = {
        .vssel = MXC_GPIO_VSSEL_VDDIOH,
        .func = MXC_GPIO_FUNC_ALT1, // Table 4 GPIO Alt. Func. Matrix
        .port = MXC_GPIO2,
        .drvstr = MXC_GPIO_DRVSTR_0,
        .mask = (TX_PIN | RX_PIN),
        .pad = MXC_GPIO_PAD_NONE
    };
    /* Some parameters are set to defaults as follows: UART Data Size - 8 bits UART Stop Bits - 1 bit UART Parity - None UART Flow Control - None UART Clock - 7.37MHz Clock (for baud > 7372800, PCLK is used) */

    if (MXC_GPIO_Config(&uart_gpio) != E_NO_ERROR) {
        printf("\nCS PIN CONFIGURATION ERROR\n");
        while (1){}
    }
    printf("\nUART Baud \t: %d Hz\n", BAUD_RATE);
    printf("\nSuccesfull Initialized UART\n\n");
}

// Send the command string byte by byte.
void nextion_send_command(const char *command) {
    // printf("About to send: %s\n", command);
    for (int i = 0; i < strlen(command); ++i) {
        MXC_UART_WriteCharacter(GLOBAL_UART_REG, command[i]);
    }
    terminate_command(); // commands must be terminated with three 0xFF bytes.
    // MXC_Delay(3000); // 3ms delay
    MXC_UART_ClearRXFIFO(GLOBAL_UART_REG); // Clear Receive for errors
}

// ends command with stop bits
void terminate_command(void) {
    for (int i = 0; i < 3; ++i) {
        MXC_UART_WriteCharacter(GLOBAL_UART_REG, 0xFF);
    }
}

void write_to_txt_component(char *objname, char *txt) {
    char dest_buff[64];
    snprintf(dest_buff, sizeof(dest_buff), "%s.txt=\"%s\"", objname, txt);
    for(int i = 0; i < 2; i++) {
        nextion_send_command(dest_buff);
    }
}

void write_to_btn_component(char *objname, char *txt) {
    char dest_buff[64];
    snprintf(dest_buff, sizeof(dest_buff), "%s.bco=%s", objname, txt);
    for(int i = 0; i < 2; i++) {
        nextion_send_command(dest_buff);
    }
}

// update the weight output text on the display
void update_weight(double weight, char *objname) {
    char weight_str[32]; // final command
    snprintf(weight_str, sizeof(weight_str), "%.2fg", weight); // combine obj, pref, weight, suff into one commands
    write_to_txt_component(objname, weight_str);
}
int global_base_array[3] = {0, 0, 0};

void update_weights(double *weights) {
    char *txt_comps[3] = {TXT_WEIGHT_0, TXT_WEIGHT_1, TXT_WEIGHT_2};
    for(int i = 0; i < 3; i++) {
        update_weight(weights[i], txt_comps[i]);
    }
}
// testing 3 load cells
double* poll_weights(void) {
     __disable_irq();
     // have to handle load cell differences
    double weight0 = get_load_cell_data(LOAD_CELL_0, load_cell_towers.load_cell_0->base_offset);
    double weight1 = get_load_cell_data(LOAD_CELL_1, load_cell_towers.load_cell_1->base_offset);
    double weight2 = get_load_cell_data(LOAD_CELL_2, load_cell_towers.load_cell_2->base_offset);
    weight_readings.weights[0] = weight0;
    weight_readings.weights[1] = weight1;
    weight_readings.weights[2] = weight2;
    update_weights(weight_readings.weights);
    __enable_irq();
    return weight_readings.weights;
}

void update_reading(int idx, uint32_t base, char *objname) {
    double weight = get_load_cell_data(idx, base);
    update_weight(weight, objname);
}

void write_game_complete(void) {
    MXC_Delay(MXC_DELAY_MSEC(500));
    char *command = "vis t6,1";
    nextion_send_command(command);
    char *exit_command = "vis t18,1";
    nextion_send_command(exit_command);
}