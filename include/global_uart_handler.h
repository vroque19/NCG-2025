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

// struct to map pages to modes and their handlers
typedef struct {
    page_t page;
    game_mode_t mode;
    void (*handler)(void);
    const char* mode_name;
} page_mode_mapping_t;

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

// void switch_mode(game_mode_t new_mode);



// typedef struct {
//     nextion_page_t page;  // Which page this component is on
//     uint8_t component;  // Component ID
//     button_handler_t handler;      // Function to call when pressed
//     const char* description;       // Description for debugging
//     project_mode_t required_mode;  // Optional: mode this button requires
// } global_screen_component_t;

// typedef struct {
//     nextion_page_t page;
//     project_mode_t default_mode;   // Mode to switch to when entering this page
//     void (*page_init)(void);  // Called when entering this page
//     void (*page_cleanup)(void);  // Called when leaving this page
//     const char* page_name;
// } page_config_t;

// // Global variables
// extern volatile project_mode_t current_mode;
// extern volatile int GLOBAL_UART_ISR_FLAG;
// extern volatile nextion_page_t current_page;

// // uart interrupt inits
// void global_uart_interrupt_init(void);
// void register_component(nextion_page_t page, uint8_t comp_id, button_handler_t handler, 
//                        const char* description, project_mode_t required_mode);
// void register_page_config(nextion_page_t page, project_mode_t default_mode, 
//                          void (*init_func)(void), void (*cleanup_func)(void), const char* name);
// void handle_component_press(nextion_page_t page, uint8_t comp_id);

// // Mode and page management
// void switch_mode(project_mode_t new_mode);
// void switch_page(nextion_page_t new_page);
// void send_page_command(nextion_page_t page);

// // System state functions
// project_mode_t get_current_mode(void);
// nextion_page_t get_current_page(void);

// // Initialization function to register all components
// void register_all_components(void);

#endif