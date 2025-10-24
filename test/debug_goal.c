#include "game_logic.h"
#include "stack.h"
#include <stdio.h>

extern const double ring_weights[];

int main(void) {
  // Test 1: 3 rings
  hanoi_init_game(3);
  
  int goal_state[NUM_TOWERS][MAX_RINGS] = {{0}};
  for (int i = 0; i < current_game.num_rings; i++) {
    goal_state[0][i] = (int)ring_weights[MAX_RINGS - current_game.num_rings + i];
  }
  
  printf("3 rings - Goal state:\n");
  for (int i = 0; i < NUM_TOWERS; i++) {
    printf("  Tower %d: %d %d %d\n", i, goal_state[i][0], goal_state[i][1], goal_state[i][2]);
  }
  
  printf("3 rings - Current state:\n");
  for (int i = 0; i < NUM_TOWERS; i++) {
    printf("  Tower %d: %d %d %d\n", i, 
      current_game.towers[i].rings[0], 
      current_game.towers[i].rings[1], 
      current_game.towers[i].rings[2]);
  }
  
  // Test 2: 1 ring
  hanoi_init_game(1);
  
  memset(goal_state, 0, sizeof(goal_state));
  for (int i = 0; i < current_game.num_rings; i++) {
    goal_state[0][i] = (int)ring_weights[MAX_RINGS - current_game.num_rings + i];
  }
  
  printf("\n1 ring - Goal state:\n");
  for (int i = 0; i < NUM_TOWERS; i++) {
    printf("  Tower %d: %d %d %d\n", i, goal_state[i][0], goal_state[i][1], goal_state[i][2]);
  }
  
  printf("1 ring - Current state:\n");
  for (int i = 0; i < NUM_TOWERS; i++) {
    printf("  Tower %d: %d %d %d\n", i, 
      current_game.towers[i].rings[0], 
      current_game.towers[i].rings[1], 
      current_game.towers[i].rings[2]);
  }
  
  printf("\nRing weights: [0]=%f [1]=%f [2]=%f\n", 
    ring_weights[0], ring_weights[1], ring_weights[2]);
  
  return 0;
}
