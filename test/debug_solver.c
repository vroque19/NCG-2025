#include "game_logic.h"
#include "history.h"
#include "queue.h"
#include "set.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_state(int state[NUM_TOWERS][MAX_RINGS]) {
  for (int i = 0; i < NUM_TOWERS; i++) {
    printf("  Tower %d: ", i);
    for (int j = 0; j < MAX_RINGS; j++) {
      printf("%d ", state[i][j]);
    }
    printf("\n");
  }
}

int main(void) {
  printf("=== Debugging Solver ===\n\n");
  
  // Test 1: Single ring
  printf("Test 1: Single ring on tower 0\n");
  hanoi_init_game(1);
  printf("Game state:\n");
  hanoi_print_game_state("Current", &current_game);
  printf("Tower data (raw):\n");
  for (int i = 0; i < NUM_TOWERS; i++) {
    printf("  Tower %d rings: ", i);
    for (int j = 0; j < MAX_RINGS; j++) {
      printf("%d ", current_game.towers[i].rings[j]);
    }
    printf("\n");
  }
  
  history_stack solution;
  init_history(&solution);
  optimal_solve(&solution);
  printf("Solution moves: %d\n\n", solution.top_idx + 1);
  
  // Test 2: Two rings
  printf("Test 2: Two rings on tower 0\n");
  hanoi_init_game(2);
  printf("Game state:\n");
  hanoi_print_game_state("Current", &current_game);
  printf("Tower data (raw):\n");
  for (int i = 0; i < NUM_TOWERS; i++) {
    printf("  Tower %d rings: ", i);
    for (int j = 0; j < MAX_RINGS; j++) {
      printf("%d ", current_game.towers[i].rings[j]);
    }
    printf("\n");
  }
  
  init_history(&solution);
  optimal_solve(&solution);
  printf("Solution moves: %d\n\n", solution.top_idx + 1);
  
  return 0;
}
