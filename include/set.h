#ifndef SET_H
#define SET_H

#include "game_logic.h"
#include <stdbool.h>

typedef struct {
  int idx;
  int items[256][NUM_TOWERS][MAX_RINGS];
} Set;

// Function declarations only
Set *set_init(void);
bool states_are_equal(int (*state)[MAX_RINGS], int (*state_to_add)[MAX_RINGS]);
void set_add(Set *visited, int (*state)[MAX_RINGS]);
bool in_set(Set *set, int (*state)[MAX_RINGS]);

#endif // SET_H
