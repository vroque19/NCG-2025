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

#ifndef __HANOI_GAME_H_
#define __HANOI_GAME_H_

#include "history.h"
#include "queue.h"
#include "set.h"
#include "stack.h"
#define NUM_TOWERS 3

// Main game state structure
typedef struct {
  uint8_t num_rings;
  tower_stack towers[3];
  uint8_t selected_tower;
  uint32_t moves_made;
  uint32_t min_moves;
  bool game_complete;
  bool is_busy;
  history_stack move_history;
} game_state_t;
// types
typedef enum {
  MANUAL_MODE,
  TOUCHSCREEN_MODE,
  AUTOMATED_MODE,
  MENU,
  SCALE
} game_mode_t;
// Move validation result
typedef enum {
  MOVE_VALID,
  MOVE_INVALID_LARGER_ON_SMALLER,
  MOVE_INVALID_EMPTY_SOURCE,
  MOVE_INVALID_SAME_TOWER,
  MOVE_INVALID_PHYSICAL_MISMATCH
} move_result_t;
extern char *txt_responses[5];
// Function prototypes
void hanoi_init_game(uint8_t num_rings);
move_result_t hanoi_validate_move(uint8_t source_tower,
                                  uint8_t destination_tower);
// bool hanoi_execute_move(uint8_t source_tower, uint8_t destination_tower);
bool hanoi_is_solved(void);
void hanoi_print_game_state(const char *name, game_state_t *game);
void hanoi_reset_game(void);
bool hanoi_execute_move(uint8_t source_tower, uint8_t destination_tower);
void optimal_solve(history_stack *solved_moves);
Queue_Entry *get_moves(int (*state)[MAX_RINGS], int *entry_idx);
extern game_state_t current_game;

#endif
