#include "test_api.h"
#include <math.h>
#include <stdio.h>

#define NUM_TOWERS 3
#define MAX_RINGS 3

void get_state_from_weights(double *weights, int (*state)[3]);

static const double RING_WEIGHTS[MAX_RINGS] = {30.0, 60.0, 110.0};
static const double VARIANCE = 15.0;

void print_state(const char *label, int (*state)[3]) {
  printf("  %s:\n", label);
  for (int tower = 0; tower < NUM_TOWERS; tower++) {
    printf("    Tower %d: [", tower);
    for (int i = 0; i < MAX_RINGS; i++) {
      if (state[tower][i] != 0) {
        printf("%d ", state[tower][i]);
      }
    }
    printf("]\n");
  }
}

void compare_states(int (*expected)[3], int (*actual)[3],
                    const char *test_name) {
  bool passed = true;
  for (int tower = 0; tower < NUM_TOWERS; tower++) {
    for (int ring = 0; ring < MAX_RINGS; ring++) {
      if (expected[tower][ring] != actual[tower][ring]) {
        passed = false;
        break;
      }
    }
    if (!passed)
      break;
  }

  if (passed) {
    printf("  ✅ PASS: %s\n", test_name);
  } else {
    printf("  ❌ FAIL: %s\n", test_name);
    print_state("Expected", expected);
    print_state("Actual", actual);
  }
}

// Test Case 1: All rings on tower 0 (starting position)
void test_all_rings_tower_0(void) {
  printf("\n--- Test Case 1: All rings on tower 0 (starting position) ---\n");

  double weights[3] = {200.0, 0.0, 0.0}; // 30 + 60 + 110 = 200
  int state[NUM_TOWERS][MAX_RINGS] = {{0}};
  int expected[NUM_TOWERS][MAX_RINGS] = {
      {110, 60, 30}, // Tower 0: all rings (bottom to top)
      {0, 0, 0},     // Tower 1: empty
      {0, 0, 0}      // Tower 2: empty
  };

  get_state_from_weights(weights, state);
  compare_states(expected, state, "Perfect weights");

  // Test with variance
  for (int variance = -15; variance <= 15; variance += 5) {
    double varied_weights[3] = {200.0 + variance, 0.0, 0.0};
    int varied_state[NUM_TOWERS][MAX_RINGS] = {{0}};
    get_state_from_weights(varied_weights, varied_state);

    char test_label[100];
    snprintf(test_label, sizeof(test_label), "Variance %+dg (total: %.0fg)",
             variance, varied_weights[0]);
    compare_states(expected, varied_state, test_label);
  }
}

// Test Case 2: Single ring on each tower
void test_one_ring_per_tower(void) {
  printf("\n--- Test Case 2: One ring on each tower ---\n");

  double weights[3] = {110.0, 60.0, 30.0};
  int state[NUM_TOWERS][MAX_RINGS] = {{0}};
  int expected[NUM_TOWERS][MAX_RINGS] = {
      {110, 0, 0}, // Tower 0: largest ring
      {60, 0, 0},  // Tower 1: medium ring
      {30, 0, 0}   // Tower 2: smallest ring
  };

  get_state_from_weights(weights, state);
  compare_states(expected, state, "Perfect weights");

  // Test each ring with variance
  printf("\n  Testing variance on tower 0 (110g ring):\n");
  for (int variance = -10; variance <= 10; variance += 5) {
    double varied_weights[3] = {110.0 + variance, 60.0, 30.0};
    int varied_state[NUM_TOWERS][MAX_RINGS] = {{0}};
    get_state_from_weights(varied_weights, varied_state);

    char test_label[100];
    snprintf(test_label, sizeof(test_label), "  Tower 0 variance %+dg (%.0fg)",
             variance, varied_weights[0]);
    compare_states(expected, varied_state, test_label);
  }

  printf("\n  Testing variance on tower 1 (60g ring):\n");
  for (int variance = -10; variance <= 10; variance += 5) {
    double varied_weights[3] = {110.0, 60.0 + variance, 30.0};
    int varied_state[NUM_TOWERS][MAX_RINGS] = {{0}};
    get_state_from_weights(varied_weights, varied_state);

    char test_label[100];
    snprintf(test_label, sizeof(test_label), "  Tower 1 variance %+dg (%.0fg)",
             variance, varied_weights[1]);
    compare_states(expected, varied_state, test_label);
  }

  printf("\n  Testing variance on tower 2 (30g ring):\n");
  for (int variance = -10; variance <= 10; variance += 5) {
    double varied_weights[3] = {110.0, 60.0, 30.0 + variance};
    int varied_state[NUM_TOWERS][MAX_RINGS] = {{0}};
    get_state_from_weights(varied_weights, varied_state);

    char test_label[100];
    snprintf(test_label, sizeof(test_label), "  Tower 2 variance %+dg (%.0fg)",
             variance, varied_weights[2]);
    compare_states(expected, varied_state, test_label);
  }
}

// Test Case 3: Two rings stacked
void test_two_ring_stacks(void) {
  printf("\n--- Test Case 3: Two rings stacked ---\n");

  // Test 110+60 on tower 0
  printf("\n  Testing 110g + 60g stack on tower 0:\n");
  double weights[3] = {170.0, 30.0, 0.0}; // 110 + 60 = 170
  int state[NUM_TOWERS][MAX_RINGS] = {{0}};
  int expected[NUM_TOWERS][MAX_RINGS] = {
      {110, 60, 0}, // Tower 0: large + medium
      {30, 0, 0},   // Tower 1: small
      {0, 0, 0}     // Tower 2: empty
  };

  get_state_from_weights(weights, state);
  compare_states(expected, state, "Perfect weights");

  for (int variance = -15; variance <= 15; variance += 5) {
    double varied_weights[3] = {170.0 + variance, 30.0, 0.0};
    int varied_state[NUM_TOWERS][MAX_RINGS] = {{0}};
    get_state_from_weights(varied_weights, varied_state);

    char test_label[100];
    snprintf(test_label, sizeof(test_label), "  Variance %+dg (%.0fg)",
             variance, varied_weights[0]);
    compare_states(expected, varied_state, test_label);
  }

  // Test 110+30 on tower 1
  printf("\n  Testing 110g + 30g stack on tower 1:\n");
  double weights2[3] = {60.0, 140.0, 0.0}; // 110 + 30 = 140
  int state2[NUM_TOWERS][MAX_RINGS] = {{0}};
  int expected2[NUM_TOWERS][MAX_RINGS] = {
      {60, 0, 0},   // Tower 0: medium
      {110, 30, 0}, // Tower 1: large + small
      {0, 0, 0}     // Tower 2: empty
  };

  get_state_from_weights(weights2, state2);
  compare_states(expected2, state2, "Perfect weights");

  for (int variance = -15; variance <= 15; variance += 5) {
    double varied_weights[3] = {60.0, 140.0 + variance, 0.0};
    int varied_state[NUM_TOWERS][MAX_RINGS] = {{0}};
    get_state_from_weights(varied_weights, varied_state);

    char test_label[100];
    snprintf(test_label, sizeof(test_label), "  Variance %+dg (%.0fg)",
             variance, varied_weights[1]);
    compare_states(expected2, varied_state, test_label);
  }

  // Test 60+30 on tower 2
  printf("\n  Testing 60g + 30g stack on tower 2:\n");
  double weights3[3] = {110.0, 0.0, 90.0}; // 60 + 30 = 90
  int state3[NUM_TOWERS][MAX_RINGS] = {{0}};
  int expected3[NUM_TOWERS][MAX_RINGS] = {
      {110, 0, 0}, // Tower 0: large
      {0, 0, 0},   // Tower 1: empty
      {60, 30, 0}  // Tower 2: medium + small
  };

  get_state_from_weights(weights3, state3);
  compare_states(expected3, state3, "Perfect weights");

  for (int variance = -15; variance <= 15; variance += 5) {
    double varied_weights[3] = {110.0, 0.0, 90.0 + variance};
    int varied_state[NUM_TOWERS][MAX_RINGS] = {{0}};
    get_state_from_weights(varied_weights, varied_state);

    char test_label[100];
    snprintf(test_label, sizeof(test_label), "  Variance %+dg (%.0fg)",
             variance, varied_weights[2]);
    compare_states(expected3, varied_state, test_label);
  }
}

// Test Case 4: All rings on tower 2 (winning position)
void test_all_rings_tower_2(void) {
  printf("\n--- Test Case 4: All rings on tower 2 (winning position) ---\n");

  double weights[3] = {0.0, 0.0, 200.0}; // 30 + 60 + 110 = 200
  int state[NUM_TOWERS][MAX_RINGS] = {{0}};
  int expected[NUM_TOWERS][MAX_RINGS] = {
      {0, 0, 0},    // Tower 0: empty
      {0, 0, 0},    // Tower 1: empty
      {110, 60, 30} // Tower 2: all rings
  };

  get_state_from_weights(weights, state);
  compare_states(expected, state, "Perfect weights");

  for (int variance = -15; variance <= 15; variance += 5) {
    double varied_weights[3] = {0.0, 0.0, 200.0 + variance};
    int varied_state[NUM_TOWERS][MAX_RINGS] = {{0}};
    get_state_from_weights(varied_weights, varied_state);

    char test_label[100];
    snprintf(test_label, sizeof(test_label), "Variance %+dg (total: %.0fg)",
             variance, varied_weights[2]);
    compare_states(expected, varied_state, test_label);
  }
}

// Test Case 5: Extreme variance test (boundary testing)
void test_extreme_variance(void) {
  printf("\n--- Test Case 5: Extreme variance (±15g boundary) ---\n");

  printf("\n  Testing 30g ring with ±15g variance:\n");
  int success_count = 0;
  int total_tests = 0;

  for (int variance = -15; variance <= 15; variance++) {
    double weights[3] = {30.0 + variance, 0.0, 0.0};
    int state[NUM_TOWERS][MAX_RINGS] = {{0}};
    int expected[NUM_TOWERS][MAX_RINGS] = {{30, 0, 0}, {0, 0, 0}, {0, 0, 0}};

    get_state_from_weights(weights, state);

    bool match = true;
    for (int t = 0; t < NUM_TOWERS; t++) {
      for (int r = 0; r < MAX_RINGS; r++) {
        if (expected[t][r] != state[t][r]) {
          match = false;
          break;
        }
      }
    }

    if (match)
      success_count++;
    total_tests++;

    if (!match || variance == -15 || variance == 0 || variance == 15) {
      char test_label[100];
      snprintf(test_label, sizeof(test_label), "  30g ring with %+dg (%.0fg)",
               variance, weights[0]);
      compare_states(expected, state, test_label);
    }
  }

  printf("\n  30g ring detection: %d/%d successful (%.1f%%)\n", success_count,
         total_tests, 100.0 * success_count / total_tests);

  // Test 60g ring
  printf("\n  Testing 60g ring with ±15g variance:\n");
  success_count = 0;
  total_tests = 0;

  for (int variance = -15; variance <= 15; variance++) {
    double weights[3] = {60.0 + variance, 0.0, 0.0};
    int state[NUM_TOWERS][MAX_RINGS] = {{0}};
    int expected[NUM_TOWERS][MAX_RINGS] = {{60, 0, 0}, {0, 0, 0}, {0, 0, 0}};

    get_state_from_weights(weights, state);

    bool match = true;
    for (int t = 0; t < NUM_TOWERS; t++) {
      for (int r = 0; r < MAX_RINGS; r++) {
        if (expected[t][r] != state[t][r]) {
          match = false;
          break;
        }
      }
    }

    if (match)
      success_count++;
    total_tests++;

    if (!match || variance == -15 || variance == 0 || variance == 15) {
      char test_label[100];
      snprintf(test_label, sizeof(test_label), "  60g ring with %+dg (%.0fg)",
               variance, weights[0]);
      compare_states(expected, state, test_label);
    }
  }

  printf("\n  60g ring detection: %d/%d successful (%.1f%%)\n", success_count,
         total_tests, 100.0 * success_count / total_tests);

  // Test 110g ring
  printf("\n  Testing 110g ring with ±15g variance:\n");
  success_count = 0;
  total_tests = 0;

  for (int variance = -15; variance <= 15; variance++) {
    double weights[3] = {110.0 + variance, 0.0, 0.0};
    int state[NUM_TOWERS][MAX_RINGS] = {{0}};
    int expected[NUM_TOWERS][MAX_RINGS] = {{110, 0, 0}, {0, 0, 0}, {0, 0, 0}};

    get_state_from_weights(weights, state);

    bool match = true;
    for (int t = 0; t < NUM_TOWERS; t++) {
      for (int r = 0; r < MAX_RINGS; r++) {
        if (expected[t][r] != state[t][r]) {
          match = false;
          break;
        }
      }
    }

    if (match)
      success_count++;
    total_tests++;

    if (!match || variance == -15 || variance == 0 || variance == 15) {
      char test_label[100];
      snprintf(test_label, sizeof(test_label), "  110g ring with %+dg (%.0fg)",
               variance, weights[0]);
      compare_states(expected, state, test_label);
    }
  }

  printf("\n  110g ring detection: %d/%d successful (%.1f%%)\n", success_count,
         total_tests, 100.0 * success_count / total_tests);
}

int main(void) {
  printf("========================================\n");
  printf("Weight Detection Accuracy Test Suite\n");
  printf("Testing get_state_from_weights() with ±15g variance\n");
  printf("Ring weights: 30g, 60g, 110g\n");
  printf("========================================\n");

  test_all_rings_tower_0();
  test_one_ring_per_tower();
  test_two_ring_stacks();
  test_all_rings_tower_2();
  test_extreme_variance();

  printf("\n========================================\n");
  printf("Test Suite Complete\n");
  printf("========================================\n");

  return 0;
}
