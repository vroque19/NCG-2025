#include "game_logic.h"
#include "history.h"
#include "queue.h"
#include "set.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  // Test: Check what states_are_equal does
  int state1[NUM_TOWERS][MAX_RINGS] = {
    {110, 0, 0},
    {0, 0, 0},
    {0, 0, 0}
  };
  
  int state2[NUM_TOWERS][MAX_RINGS] = {
    {0, 0, 0},
    {0, 0, 0},
    {110, 60, 30}
  };
  
  printf("State1 (1 ring on tower 0):\n");
  for (int i = 0; i < NUM_TOWERS; i++) {
    printf("  Tower %d: %d %d %d\n", i, state1[i][0], state1[i][1], state1[i][2]);
  }
  
  printf("\nState2 (goal - 3 rings on tower 2):\n");
  for (int i = 0; i < NUM_TOWERS; i++) {
    printf("  Tower %d: %d %d %d\n", i, state2[i][0], state2[i][1], state2[i][2]);
  }
  
  bool equal = states_are_equal(state1, state2);
  printf("\nAre they equal? %d (should be 0)\n", equal);
  
  // Now check if 1 ring on tower 2 matches goal
  int state3[NUM_TOWERS][MAX_RINGS] = {
    {0, 0, 0},
    {0, 0, 0},
    {110, 0, 0}
  };
  
  printf("\n\nState3 (1 ring on tower 2):\n");
  for (int i = 0; i < NUM_TOWERS; i++) {
    printf("  Tower %d: %d %d %d\n", i, state3[i][0], state3[i][1], state3[i][2]);
  }
  
  equal = states_are_equal(state3, state2);
  printf("\nDoes 1 ring on tower 2 match 3-ring goal? %d (should be 0)\n", equal);
  
  return 0;
}
