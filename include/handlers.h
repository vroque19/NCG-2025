#ifndef __HANDLERS_H_
#define __HANDLERS_H_
#include <stddef.h>
#include <mode_touchscreen.h>

#define MOVE_COUNT_ID "n0"
#define STATUS_TXT "t6"
// MAIN MENU IDs
#define PAGE_TOUCHSCREEN_ID 0x04
#define PAGE_MANUAL_ID 0x03
#define PAGE_AUTOMATED_ID 0x06

// TOUCHSCREEN IDs
#define TOWER_0_ID 0x07
#define TOWER_1_ID 0x08
#define TOWER_2_ID 0x09
#define EXIT_TOUCHSCREEN_ID 0x0E

// MANUAL IDs
#define SOLENOID_ID 0x07
#define EXIT_MANUAL_ID 0x09

// AUTOMATED IDs
#define START_AUTOMATED_ID 0x08
#define EXIT_AUTOMATED_ID 0x07


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
    page_t page;
    uint8_t component;
    void (*handler_function)(void);
} screen_component;


/* For functions used by multiple files */
void exit_to_main_menu(void);
void solenoid_handler(void);
void update_count_txt(void);
void handle_tower_0_btn(void);
void handle_tower_1_btn(void);
void handle_tower_2_btn(void);
// void move_0_to_1_handler(void);
void switch_page_touchscreen(void);
void switch_page_manual(void);
void switch_page_automated(void);
void start_automated(void);

#endif