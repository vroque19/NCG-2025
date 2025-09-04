#ifndef __HANDLERS_H_
#define __HANDLERS_H_
// #include <stddef.h>
#include "mode_touchscreen.h"
// #include "stack.h"
#include "game_logic.h"
#include "solenoid_driver.h"
#include "moves.h"



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

void update_towers(void);
/* For functions used by multiple files */
void exit_to_main_menu(void);
void increment_count(void);
// touchscreen functions
void handle_tower_0_btn(void);
void handle_tower_1_btn(void);
void handle_tower_2_btn(void);
// void move_0_to_1_handler(void);
// Main Menu to page functions
void switch_page_touchscreen(void);
void switch_page_manual(void);
void switch_page_automated(void);
void switch_mode(game_mode_t new_mode);
void switch_page(game_mode_t new_page);
void nextion_write_game_state(game_state_t *game);
void get_string_from_rings(int top_idx, uint8_t *tower_rings, char *tower_str, uint8_t str_size);
// Auto page functions
void start_automated(void);

void solenoid_handler(void);
#endif