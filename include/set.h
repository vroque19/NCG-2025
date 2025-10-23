#include "game_logic.h"
#include "stack.h"
#include <memory.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
typedef struct {
  int idx;
  int items[256][MAX_RINGS];
} Set;

Set *set_init(void) {
  Set *mem = (Set *)malloc(sizeof(Set));
  memset(mem, 0, sizeof(Set));
  mem->idx = 0;
  return mem;
}
// checks if states are equal
bool states_are_equal(int (*state)[MAX_RINGS], int (*state_to_add)[MAX_RINGS]) {
  for (int j = 0; j < NUM_TOWERS; ++j) {
    for (int k = 0; k < MAX_RINGS; ++k) {
      // triple nested forloop xd
      if (state[j][k] != state_to_add[j][k]) {
        return false;
      }
    }
  }
  return true;
}
// checks if we've already seen this state before
void set_add(Set *visited, int (*state)[MAX_RINGS]) {
  for (int i = 0; i <= visited->idx; ++i) {
    if (states_are_equal(&visited->items[i], state)) {
      return;
    }
  }
  memcpy(visited->items[visited->idx], state, sizeof(state[0]) * 3);
  visited->idx += 1;
  return;
}

bool in_set(Set *set, int (*state)[MAX_RINGS]) {
  for (int i = 0; i < set->idx; ++i) {
    if (states_are_equal(&set->items[i], state)) {
      return true;
    }
  }
  return false;
}
