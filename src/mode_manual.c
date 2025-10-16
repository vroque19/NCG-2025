#include "mode_manual.h"
#include "TMC5272.h"
#include "game_logic.h"
#include "global_uart_handler.h"
#include "nextion.h"
#include "moves.h"
#include <stdbool.h>

// too slow
void update_tower_weights(void) {
    double *weights = poll_weights();
    for(int i = 0; i < 3; i++) {
        printf("Updated Tower %d Weight: %d\n", weights[i]);
        current_game.towers[i].total_weight = weights[i];
    }
}

// returns the source tower of the ring and updates tower weights
int get_tower_from_weight_delta(void) {
  double *weights = poll_weights();
  int tower = 10;
    for(int i = 0; i < 3; i++) {
        current_game.towers[i].total_weight = weights[i];
        // printf("Updated Tower %d Weight: %d\n", weights[i]);
        if (abs(current_game.towers[i].total_weight - weights[i]) >= 30) {
            tower = i;
        } 
    }
    return tower;
}




