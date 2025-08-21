// global_uart_handler.h
#ifndef _GLOBAL_UART_HANDLER_H_
#define _GLOBAL_UART_HANDLER_H_

#include "nextion.h"
#include "4131.h"
#include "mode_touchscreen.h"
#include <stdint.h>

#define PAGE_AUTOMATED
#define TOWER_0_ID 0x07
#define TOWER_1_ID 0x08
#define TOWER_2_ID 0x09
#define TOGGLE_1TO2_ID 0x0A
#define PAGE_MAIN_MENU_ID 0x00
#define PAGE_TOUCHSCREEN_ID 0x01
#define PAGE_MANUAL_ID 0x02
#define PAGE_AUTOMATED_ID 0x03


// types
typedef enum {
    MANUAL_MODE,
    TOUCHSCREEN_MODE,
    AUTOMATED_MODE
} game_mode_t;

typedef enum {
    PAGE_MAIN_MENU,
    PAGE_TOUCHSCREEN,
    PAGE_MANUAL,
    PAGE_AUTOMATED
} page_t;

// struct for screen components
typedef struct {
    uint8_t page;
    uint8_t component;
    void (*handler_function)(void);
} screen_component;

// Global variables
extern volatile game_mode_t current_mode;
extern volatile int GLOBAL_UART_ISR_FLAG;
extern volatile page_t current_page;
// struct to map pages to modes and their handlers
typedef struct {
    page_t page;
    game_mode_t mode;
    void (*handler)(void);
    const char* mode_name;
} page_mode_mapping_t;


void global_uart_init(void);
void uart_isr(void);
void global_uart_callback(mxc_uart_req_t *req, int error);
uint8_t get_event(uint8_t *buff);
page_t get_page(uint8_t *buff);
uint8_t get_component(uint8_t *buff);
void global_uart_interrupt_disable(void);
void handle_touch_event(uint8_t *rx_data);
void switch_mode(game_mode_t new_mode);
const char* get_mode_name(game_mode_t mode) ;
const char* get_page_name(page_t page);
void global_uart_main_loop(void);

// typedef enum {
//     MANUAL_MODE,
//     TOUCHSCREEN_MODE,
//     AUTOMATED_MODE
// } project_mode_t;

// typedef enum {
//     PAGE_MAIN = 0x00,
//     PAGE_TOUCHSCREEN = 0x01,
//     PAGE_MANUAL = 0x02,
//     PAGE_AUTOMATED = 0x03,
//     // Add more pages as needed
// } nextion_page_t;

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