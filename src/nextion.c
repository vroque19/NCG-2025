#include "nextion.h"


volatile uint32_t *uart_int_en, *uart_int_flags; 

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
    MXC_Delay(5000); // 5ms delay
    MXC_UART_ClearRXFIFO(GLOBAL_UART_REG); // Clear Receive for errors
}

// ends command with stop bits
void terminate_command(void) {
    for (int i = 0; i < 3; ++i) {
        MXC_UART_WriteCharacter(GLOBAL_UART_REG, 0xFF);
    }
}

void write_to_txt_component(char* objname, char* txt) {
    char dest_buff[64];
    snprintf(dest_buff, sizeof(dest_buff), "%s.txt=\"%s\"", objname, txt);
    for(int i = 0; i < 2; i++) {
        nextion_send_command(dest_buff);
    }
}

// update the weight output text on the display
void update_weight(double weight, char *objname) {
    char weight_str[32]; // final command
    snprintf(weight_str, sizeof(weight_str), "%.2fg", weight); // combine obj, pref, weight, suff into one commands
    printf("weight str: %s\n", weight_str);
    write_to_txt_component(objname, weight_str);
}

// testing 3 load cells
void poll_weights(void) {
    double weight0 = get_load_cell_data(LOAD_CELL_0, global_base_array[LOAD_CELL_0]);
    double weight1 = get_load_cell_data(LOAD_CELL_1, global_base_array[LOAD_CELL_1]);
    double weight2 = get_load_cell_data(LOAD_CELL_2, global_base_array[LOAD_CELL_2]);
    update_weight(weight2, TXT_WEIGHT_2);
    update_weight(weight1, TXT_WEIGHT_1);
    update_weight(weight0, TXT_WEIGHT_0);
}

void update_reading(int idx, uint32_t base, char *objname) {
    double weight = get_load_cell_data(idx, base);
    update_weight(weight, objname);
}
