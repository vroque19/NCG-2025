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

#ifndef __HANDLERS_H_
#define __HANDLERS_H_
// #include <stddef.h>
#include "mode_manual.h"
#include "mode_touchscreen.h"
// #include "stack.h"
#include "TMC5272.h"
#include "game_logic.h"
#include "moves.h"
#include "solenoid_driver.h"
#include <stdint.h>

typedef enum {
  PAGE_MAIN_MENU,
  PAGE_MANUAL,
  PAGE_TOUCHSCREEN,
  PAGE_AUTOMATED,
  PAGE_SCALE
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
void run_manual_mode_logic(tmc5272_dev_t *tmc_x, tmc5272_dev_t *tmc_y,
                           tmc5272_dev_t *tmc_tc);
void switch_page_touchscreen(void);
void switch_page_manual(void);
void switch_page_automated(void);
void switch_page_scale(void);
void switch_mode(game_mode_t new_mode);
void switch_page(game_mode_t new_page);
void nextion_write_game_state(game_state_t *game);
void get_string_from_rings(int top_idx, uint8_t *tower_rings, char *tower_str,
                           uint8_t str_size);
void auto_solve_hanoi(int num_rings, int source, int dest);
// Auto page functions
void start_automated(void);
void start_cal(void);
void solenoid_handler(void);
void weigh_scale_routine(void);
void get_state_from_weights(double *weights, int (*state)[NUM_TOWERS]);
#endif
