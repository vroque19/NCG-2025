#ifndef __HANOI_GAME_H_
#define __HANOI_GAME_H_

#include "stack.h"
#include "history.h"

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
    AUTOMATED_MODE, MENU
} game_mode_t;
// Move validation result
typedef enum {
    MOVE_VALID,
    MOVE_INVALID_LARGER_ON_SMALLER,
    MOVE_INVALID_EMPTY_SOURCE,
    MOVE_INVALID_SAME_TOWER,
    MOVE_INVALID_PHYSICAL_MISMATCH
} move_result_t;
extern char* txt_responses[5];
// Function prototypes
void hanoi_init_game(uint8_t num_rings);
move_result_t hanoi_validate_move(uint8_t source_tower, uint8_t destination_tower);
// bool hanoi_execute_move(uint8_t source_tower, uint8_t destination_tower);
bool hanoi_is_solved(void);
void hanoi_print_game_state(const char* name, game_state_t *game);
void hanoi_reset_game(void);

extern game_state_t current_game;

#endif