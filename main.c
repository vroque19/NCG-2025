#include <stdio.h>
#include <string.h>
#include "uart_regs.h"
#include "mxc_sys.h"
#include "mxc_device.h"
#include "mxc_delay.h"
#include "uart.h"

#define BAUD_RATE 9600

int nextion_init(mxc_uart_regs_t *uart) {
    if(!uart) {
        return E_BAD_PARAM;
    }
    MXC_UART_Init(uart, BAUD_RATE);
    MXC_UART_ClearRXFIFO(uart);
    MXC_UART_ClearTXFIFO(uart);

}

int main(void) {
    mxc_uart_regs_t *uart;
    MXC_UART_Shutdown(uart);
}