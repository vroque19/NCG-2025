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
    // Some parameters are set to defaults as follows: UART Data Size - 8 bits UART Stop Bits - 1 bit UART Parity - None UART Flow Control - None UART Clock - 7.37MHz Clock (for baud > 7372800, PCLK is used)

    if (MXC_GPIO_Config(&uart_gpio) != E_NO_ERROR) {
        printf("\nCS PIN CONFIGURATION ERROR\n");
        while (1){}
    }
    printf("\nUART Baud \t: %d Hz\n", BAUD_RATE);
    printf("\nSuccesfull Initialized UART\n\n");
}

// /* ISR for the UART interrupts to set flags*/
// void UART1_ISR(void) {
//     // MXC_UART_DisableInt(GLOBAL_UART_REG, RX_LVL);
//     printf("~~~~~~ In ISR. Flag = %d ~~~~~~\n\n", UART_ISR_FLAG);
//     unsigned int flags = MXC_UART_GetFlags(GLOBAL_UART_REG);
//     printf("Flags: %d \n", flags);
//     // Only process if we're not already processing
//     if (( flags & RX_LVL) && (UART_ISR_FLAG == 0)) {
//         printf("Processing RX threshold interrupt\n");
//         // UART_ISR_FLAG = 1;
//         MXC_UART_AsyncHandler(GLOBAL_UART_REG);
//         // Clear only the RX threshold flag (bit 4)
//         MXC_UART_ClearFlags(GLOBAL_UART_REG, RX_LVL);
//     }
//     else {
//         printf("------Ignoring interrupt - flags=0x%X, ISR_FLAG=%d\n-----",  flags, UART_ISR_FLAG);
//         // Clear other flags to prevent repeated interrupts
//         MXC_UART_ClearFlags(GLOBAL_UART_REG,  flags & ~RX_LVL);
//     }
    
//     // MXC_UART_AsyncHandler(GLOBAL_UART_REG);
//     // MXC_UART_ClearFlags(GLOBAL_UART_REG, 1U<<4);
//     MXC_UART_EnableInt(GLOBAL_UART_REG, RX_LVL);
//     MXC_UART_ClearRXFIFO(GLOBAL_UART_REG);
// }

// void readCallback(mxc_uart_req_t *req, int error) {
//     printf(">>>> Callback function <<<< \n\n");
//     if (error == E_NO_ERROR) {
//         // Transaction completed successfully
//         printf("UART transaction completed successfully\n");
//         printf("Received %d bytes\n", req->rxCnt);
        
//         // Signal that data is ready to be processed
//         UART_ISR_FLAG = 1;
        
//         printf("Raw data: ");
//         print_buff(req->rxData, req->rxCnt);
//         printf("\n");
        
//     } else {
//         // Handle errors
//         printf("UART transaction error: %d\n", error);
        
//         switch(error) {
//             case E_BUSY:
//                 printf("UART busy\n");
//                 break;
//             case E_BAD_PARAM:
//                 printf("Bad parameter\n");
//                 break;
//             case E_SHUTDOWN:
//                 printf("UART shutdown\n");
//                 break;
//             default:
//                 printf("Unknown error\n");
//                 break;
//         }
//     }
        
// }

// void nextion_int_init(void) {
//     uart_int_en = (MXC_BASE_UART1 + MXC_R_UART_INT_EN);
//     uart_int_flags = (MXC_BASE_UART1 + MXC_R_UART_INT_FL);
//         // enable interrupts
//     NVIC_ClearPendingIRQ(UART1_IRQn);
//     NVIC_DisableIRQ(UART1_IRQn);
//     MXC_NVIC_SetVector(UART1_IRQn, UART1_ISR);
//     NVIC_EnableIRQ(UART1_IRQn);
//     MXC_UART_SetRXThreshold(GLOBAL_UART_REG, BYTES);
//     MXC_UART_EnableInt(GLOBAL_UART_REG, RX_LVL);
    
// }

// Send the command string byte by byte.
void nextion_send_command(const char *command) {
    printf("About to send: %s\n", command);
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
