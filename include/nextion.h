#ifndef _NEXTION_H_
#define _NEXTION_H_
#include "4131.h"
#include <stdio.h>
#include <string.h>
// #include "uart_regs.h"
// #include "mxc_sys.h"
#include "nvic_table.h"
#include "load_cell.h"
#include "mxc_delay.h"
#include "mxc_device.h"
#include "mxc_pins.h"
#include "nextion.h"
#include "uart.h"

#define BAUD_RATE 9600
#define GLOBAL_UART_REG   MXC_UART1
#define UART_PORT     MXC_GPIO_PORT_2
#define TX_PIN         MXC_GPIO_PIN_16
#define RX_PIN     MXC_GPIO_PIN_14
#define TXT_WEIGHT_0 "t0"
#define TXT_WEIGHT_1 "t1"
#define TXT_WEIGHT_2 "t2"
#define TOUCH_EVENT 0x65
// #define TOWER_0_ID 0x07
// #define TOWER_1_ID 0x08
// #define TOWER_2_ID 0x09
#define TOWER_0_BTN_ID "b0"
#define TOWER_1_BTN_ID "b2"
#define TOWER_2_BTN_ID "b3"
#define TOWER_0_ID 0x0E
#define TOWER_1_ID 0x0F
#define TOWER_2_ID 0x10
#define BUTTON_OFF "61309"
#define BUTTON_ON "57335"
// #define BUTTON_ON2 "562"
#define TOGGLE_1TO2_ID 0x0A
#define PAGE1_ID 0x01
#define MAIN_TXT_BOX "t8"
#define MOVE_COUNT_ID "n0"
#define STATUS_TXT "t6"
// MAIN MENU IDs
#define PAGE_TOUCHSCREEN_ID 0x04
#define PAGE_MANUAL_ID 0x03
#define PAGE_AUTOMATED_ID 0x06

#define EXIT_TOUCHSCREEN_ID 0x0A

// MANUAL IDs
#define SOLENOID_ID 0x07
#define EXIT_MANUAL_ID 0x09

// AUTOMATED IDs
#define START_AUTOMATED_ID 0x08
#define EXIT_AUTOMATED_ID 0x07


/* INTERRUPT globals */
extern volatile int UART_ISR_FLAG; // 1 when interrupt occurs

#define BYTES 7 // expects 7 bytes
#define RX_LVL 1U<<4 // threshold interrupt
extern volatile uint32_t *uart_int_en, *uart_int_flags; // debugging


void update_reading(int idx, uint32_t base, char *objname);  

void nextion_init(void);
void nextion_send_command(const char *command); 
void terminate_command(void);
void update_weight(double weight, char *objname);
void poll_weights(void);
void write_to_txt_component(char* objname, char* txt);
void write_to_btn_component(char *objname, char *txt);




#endif