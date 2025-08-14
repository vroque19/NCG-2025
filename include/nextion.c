#include "nextion.h"


volatile uint32_t *uart_int_en, *uart_int_flags; 
void nextion_init(void) {
    // INIT UART
    if(MXC_UART_Init(NEXTION_UART_REG, BAUD_RATE) != E_NO_ERROR) {
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
    // Some parameters are set to defaults as follows: UART Data Size - 8 bits UART Stop Bits - 1 bit UART Parity - None UART Flow Control - None UART Clock - 7.37MHz Clock (for baud > 7372800, PCLK is used)

    if (MXC_GPIO_Config(&uart_gpio) != E_NO_ERROR) {
        printf("\nCS PIN CONFIGURATION ERROR\n");
        while (1){}
    }
    printf("\nUART Baud \t: %d Hz\n", BAUD_RATE);
    printf("\nSuccesfull Initialized UART\n\n");
}

void UART1_ISR(void) {
    printf("~~~~~~ In ISR. Flag = %d ~~~~~~\n\n", UART_ISR_FLAG);
    printf("Flags: %d \n", MXC_UART_GetFlags(NEXTION_UART_REG));
    UART_ISR_FLAG = 1;
    MXC_UART_AsyncHandler(NEXTION_UART_REG);


    MXC_UART_ClearFlags(NEXTION_UART_REG, 1U<<4);
    NEXTION_UART_REG->int_en = 0x10; 
    MXC_UART_ClearRXFIFO(NEXTION_UART_REG);
    
}

void readCallback(mxc_uart_req_t *req, int error) {
  printf(">>>> Callback function <<<< \n\n");
  UART_ISR_FLAG = error;
}

void nextion_int_init(void) {
    uart_int_en = (MXC_BASE_UART1 + MXC_R_UART_INT_EN);
    uart_int_flags = (MXC_BASE_UART1 + MXC_R_UART_INT_FL);
        // enable interrupts
    NVIC_ClearPendingIRQ(UART1_IRQn);
    NVIC_DisableIRQ(UART1_IRQn);
    MXC_NVIC_SetVector(UART1_IRQn, UART1_ISR);
    NVIC_EnableIRQ(UART1_IRQn);
    MXC_UART_SetRXThreshold(NEXTION_UART_REG, BYTES);
    MXC_UART_EnableInt(NEXTION_UART_REG, RX_LVL);
    
}
void nextion_send_command(const char *command) {
    // Send the command string byte by byte.
    for (int i = 0; i < strlen(command); ++i) {
        MXC_UART_WriteCharacter(NEXTION_UART_REG, command[i]);
    }
    terminate_command(); // commands must be terminated with three 0xFF bytes.
}

void terminate_command(void) {
    // 3 0xFF bytes
    for (int i = 0; i < 3; ++i) {
        MXC_UART_WriteCharacter(NEXTION_UART_REG, 0xFF);
    }
    // MXC_Delay(MXC_DELAY_MSEC(50));
    // printf("Sent\n");
}

// update the weight output text on the display
void update_weight(double weight, char *objname) {
    char prefix[] = ".txt=\"";
    char dest_buff[50]; // final command
    char suffix[] = "\"";
    snprintf(dest_buff, sizeof(dest_buff), "%s%s%.2f%s", objname, prefix, weight, suffix); // combine obj, pref, weight, suff into one commands
    // printf("Combined command: %s\n", dest_buff); // debugging what is being sent
    nextion_send_command(dest_buff);
}

// testing 3 load cells
void poll_weights(uint32_t base0, uint32_t base1, uint32_t base2) {
    double weight0 = get_load_cell_data(LOAD_CELL_0, base0);
    double weight1 = get_load_cell_data(LOAD_CELL_1, base1);
    double weight2 = get_load_cell_data(LOAD_CELL_2, base2);
    // MXC_Delay(MXC_DELAY_MSEC(900));
    update_weight(weight2, TXT_WEIGHT_2);
    // // MXC_Delay(MXC_DELAY_MSEC(2000));
    update_weight(weight1, TXT_WEIGHT_1);
    // MXC_Delay(MXC_DELAY_MSEC(2000));
    update_weight(weight0, TXT_WEIGHT_0);
}

void update_reading(int idx, uint32_t base, char *objname) {
    double weight = get_load_cell_data(idx, base);
    update_weight(weight, objname);
}

uint8_t get_event(uint8_t *buff) {
    return buff[0];
}

uint8_t get_page(uint8_t *buff) {
    return buff[1];
}

uint8_t get_component(uint8_t *buff) {
    return buff[2];
}