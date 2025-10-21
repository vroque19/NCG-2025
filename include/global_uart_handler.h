/* 
MIT License

Copyright (c) 2025 Analog Devices

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

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