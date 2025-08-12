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
#define TXT_WEIGHT_0 "t0"
#define TXT_WEIGHT_1 "t1"
#define TXT_WEIGHT_2 "t2"
void nextion_init(void);
void nextion_send_command(const char *command); 
// "n0.val=4343" or label0.txt="jfjjf"
void terminate_command(void);
void update_weight(float weight, char *objname);


#endif