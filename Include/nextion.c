#include "nextion.h"


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

    if (MXC_GPIO_Config(&uart_gpio) != E_NO_ERROR)
    {
        printf("\nCS PIN CONFIGURATION ERROR\n");
        while (1)
        {
        }
    }
    printf("\nUART Baud \t: %d Hz\n", BAUD_RATE);
    // printf("Baud0: %d, Baud1%d\n", NEXTION_UART_REG->baud0, NEXTION_UART_REG->baud1);
    // MXC_GPIO_Config(&uart_gpio);
    printf("\nSuccesfull Initialized UART\n\n");
}


void nextion_send_command(const char *command) {
    // Send the command string byte by byte.
    char prefix[] = "t0.txt=";
    for (int i = 0; i < strlen(prefix); ++i) {
        MXC_UART_WriteCharacter(NEXTION_UART_REG, prefix[i]);
    }
    for (int i = 0; i < strlen(command); ++i) {
        MXC_UART_WriteCharacter(NEXTION_UART_REG, command[i]);
    }
    terminate_command();
    // NEXTION commands must be terminated with three 0xFF bytes.
}

void terminate_command(void) {
    for (int i = 0; i < 3; ++i) {
        MXC_UART_WriteCharacter(NEXTION_UART_REG, 0xFF);
    }
    MXC_Delay(MXC_DELAY_MSEC(50));
}

