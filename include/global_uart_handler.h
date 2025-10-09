// global_uart_handler.h
#ifndef _GLOBAL_UART_HANDLER_H_
#define _GLOBAL_UART_HANDLER_H_

#include "nextion.h"
#include "4131.h"
#include "mode_touchscreen.h"
#include "handlers.h"
// #include <stdint.h>

// Global variables
extern volatile int GLOBAL_UART_ISR_FLAG;
extern volatile game_mode_t current_mode;
extern volatile page_t current_page;


/**
 * @brief Initializes UART interrupts to read data from the nextion display
 * @return void
 */
void global_uart_init(void);
void uart_isr(void);
void global_uart_callback(mxc_uart_req_t *req, int error);
uint8_t get_event(uint8_t *buff);
page_t get_page(uint8_t *buff);
uint8_t get_component(uint8_t *buff);
void global_uart_interrupt_disable(void);
void handle_touch_event(uint8_t *rx_data);
const char* get_mode_name(game_mode_t mode) ;
const char* get_page_name(page_t page);
void global_uart_main_loop(void);

#endif