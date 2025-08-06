#ifndef _NEXTION_H_
#define _NEXTION_H_
#include <stdio.h>
#include <string.h>
#include "uart_regs.h"
#include "mxc_sys.h"
#include "mxc_device.h"
#include "mxc_delay.h"
#include "uart.h"

#define BAUD_RATE 9600
#define NEXTION_UART_REG   MXC_UART1
#define UART_PORT     MXC_GPIO_PORT_2
#define TX_PIN         MXC_GPIO_PIN_16
#define RX_PIN     MXC_GPIO_PIN_14

void nextion_init(void);
void send_test(void);
void nextion_send_command(const char *command);
void nextion_send_hex(const char* command);
int send_req(void);
void print_buff(const uint8_t *buff, size_t len);
void send_read_test(void);
void terminate_command(void);

#endif