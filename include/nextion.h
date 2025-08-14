#ifndef _NEXTION_H_
#define _NEXTION_H_
#include "4131.h"
#include <stdio.h>
#include <string.h>
#include "uart_regs.h"
#include "mxc_sys.h"
#include "nvic_table.h"
#include "load_cell.h"
#include "mxc_delay.h"
#include "mxc_device.h"
#include "mxc_pins.h"
#include "nextion.h"
#include "uart.h"

#define BAUD_RATE 9600
#define NEXTION_UART_REG   MXC_UART1
#define UART_PORT     MXC_GPIO_PORT_2
#define TX_PIN         MXC_GPIO_PIN_16
#define RX_PIN     MXC_GPIO_PIN_14
#define TXT_WEIGHT_0 "t0"
#define TXT_WEIGHT_1 "t1"
#define TXT_WEIGHT_2 "t2"
#define TOUCH_EVENT 0x65

/* INTERRUPT globals */
extern volatile int UART_ISR_FLAG; // 1 when interrupt occurs

#define BYTES 7 // expects 7 bytes
#define RX_LVL 1U<<4 // threshold interrupt
extern volatile uint32_t *uart_int_en, *uart_int_flags; // debugging


void update_reading(int idx, uint32_t base, char *objname);  

void nextion_init(void);
void nextion_int_init(void);
void UART1_ISR(void);
void readCallback(mxc_uart_req_t *req, int error);
void nextion_send_command(const char *command); 
void terminate_command(void);
void update_weight(double weight, char *objname);
void poll_weights(uint32_t base0, uint32_t base1, uint32_t base2);
uint8_t get_event(uint8_t *buff);
uint8_t get_page(uint8_t *buff);
uint8_t get_component(uint8_t *buff);




#endif