#include <stdbool.h>
#include <math.h>

#define NUM_TOWERS 3
#define MAX_RINGS 3

void get_state_from_weights(double *weights, int (*state)[3]) {
  const double RING_WEIGHTS[MAX_RINGS] = {30.0, 60.0, 110.0};
  const double TOLERANCE = 15.5;

  // Initialize state to zeros
  for (int i = 0; i < NUM_TOWERS; i++) {
    for (int j = 0; j < MAX_RINGS; j++) {
      state[i][j] = 0;
    }
  }

  // Track which rings we've assigned
  bool ring_assigned[MAX_RINGS] = {false, false, false};

  // For each tower, determine which rings are on it
  for (int tower = 0; tower < NUM_TOWERS; tower++) {
    double remaining_weight = weights[tower];
    int stack_idx = 0;

    // Try to match rings from largest to smallest (bottom to top)
    for (int ring = MAX_RINGS - 1; ring >= 0; ring--) {
      if (remaining_weight < RING_WEIGHTS[0] * 0.5) {
        break;
      }
      if (!ring_assigned[ring] &&
          fabs(remaining_weight - RING_WEIGHTS[ring]) < TOLERANCE) {
        // This ring is on this tower
        state[tower][stack_idx] = (int)RING_WEIGHTS[ring];
        remaining_weight -= RING_WEIGHTS[ring];
        ring_assigned[ring] = true;
        stack_idx++;
      } else if (!ring_assigned[ring] &&
                 remaining_weight >= RING_WEIGHTS[ring]) {
        // Check if this ring plus others could sum to remaining weight
        double test_weight = RING_WEIGHTS[ring];

        for (int smaller = ring - 1; smaller >= 0; smaller--) {
          if (!ring_assigned[smaller] && test_weight < remaining_weight) {
            test_weight += RING_WEIGHTS[smaller];
          }
        }

        if (fabs(remaining_weight - test_weight) < TOLERANCE ||
            fabs(remaining_weight - RING_WEIGHTS[ring]) <
                remaining_weight / 2) {
          state[tower][stack_idx] = (int)RING_WEIGHTS[ring];
          remaining_weight -= RING_WEIGHTS[ring];
          ring_assigned[ring] = true;
          stack_idx++;
        }
      }
    }
  }
}
