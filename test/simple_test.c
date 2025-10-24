#include "game_logic.h"
#include "history.h"
#include <stdio.h>

int main(void) {
  // Simple test: 1 ring on tower 2, should reset to tower 0
  hanoi_init_game(1);
  
  // Manually put ring on tower 2
  init_stack(&current_game.towers[0]);
  init_stack(&current_game.towers[1]);
  init_stack(&current_game.towers[2]);
  push_tower(&current_game.towers[2], 110);
  
  printf("Starting state:\n");
  for (int i = 0; i < NUM_TOWERS; i++) {
    printf("  Tower %d: %d\n", i, current_game.towers[i].rings[0]);
  }
  
  history_stack solution;
  init_history(&solution);
  
  optimal_solve(&solution);
  
  printf("Solution has %d moves\n", solution.top_idx + 1);
  for (int i = 0; i <= solution.top_idx; i++) {
    printf("  Move %d: %d -> %d\n", i+1, solution.moves[i].source, solution.moves[i].destination);
  }
  
  return 0;
}
