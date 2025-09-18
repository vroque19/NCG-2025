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
// #include "nextion.h"
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

#define TOWER_0_BTN_ID "b0"
#define TOWER_1_BTN_ID "b2"
#define TOWER_2_BTN_ID "b3"
#define TOWER_0_ID 0x14
#define TOWER_1_ID 0x15
#define TOWER_2_ID 0x16
#define BUTTON_OFF "61309" // light blue
#define BUTTON_ON "562" // dark blue
#define TOGGLE_1TO2_ID 0x0A
#define PAGE1_ID 0x01
#define MAIN_TXT_BOX "t8"
#define MOVE_COUNT_ID "n0"
#define STATUS_TXT "t6" // not in use
// MAIN MENU IDs
#define PAGE_TOUCHSCREEN_ID 0x04
#define PAGE_MANUAL_ID 0x03
#define PAGE_AUTOMATED_ID 0x05

#define EXIT_TOUCHSCREEN_ID 0x0D

// MANUAL PAGE IDs
#define SOLENOID_ID 0x01
#define EXIT_MANUAL_ID 0x09

// AUTOMATED PAGE IDs
#define START_AUTOMATED_ID 0x08
#define EXIT_AUTOMATED_ID 0x07

#define RING_COLOR_DEFAULT 562
#define RING_COLOR_SELECTED 63115

/* INTERRUPT globals */
extern volatile int UART_ISR_FLAG; // 1 when interrupt occurs

#define BYTES 7 // expects 7 bytes
#define RX_LVL 1U<<4 // threshold interrupt
extern volatile uint32_t *uart_int_en, *uart_int_flags; // debugging


/**
 * @brief Wrapper for update_weight to specify which load cell is being targeted
 * @param idx the load cell index
 * @param base the calibration factor for the load cell
 * @param objname name of the weight scale txt component
 * @return void
 */
void update_reading(int idx, uint32_t base, char *objname);  

/**
 * @brief Initializes UART for writing to the nextion display
 * @return void
 */
void nextion_init(void);
void nextion_send_command(const char *command); 
/**
 * @brief Sends 3 0xFF terminating bytes 
 * @return void
 */
void terminate_command(void);

void update_weight(double weight, char *objname);

/**
 * @brief Reads from 4131 channels in sequence, converts to grams, and writes weights to the display
 * @return array of weights
 */
double* poll_weights(void);

/**
 * @brief Writes string to a Nextion TXT component
 * @param objname name of the txt component
 * @param txt string to write to the display txt component
 * @return void
 */
void write_to_txt_component(char* objname, char* txt);

/**
 * @brief Writes string to a Nextion BTN component
 * @param objname name of the btn component
 * @param txt string to write to the display btn component
 * @return void
 */
void write_to_btn_component(char *objname, char *txt);




#endif