#include "TMC5272.h"
#include "moves.h"
#include "game_logic.h"
#include "load_cell.h"
#include <stdint.h>

// returns the current_game selected tower
int get_source_tower(int pos_x);

// returns the tower to move to
int get_destination_tower(int pos_x);

// get the weight difference from before and after movement
void tower_weight_delta(uint16_t prev_weight, uint16_t curr_weight);

void update_tower_weights(void);

int get_tower_from_weight_delta(void);