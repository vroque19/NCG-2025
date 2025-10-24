#include "game_logic.h"
#include "history.h"
#include "queue.h"
#include "set.h"
#include "stack.h"
#include "test_api.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_state(int state[NUM_TOWERS][MAX_RINGS]) {
  for (int i = 0; i < NUM_TOWERS; i++) {
    printf("  Tower %d: ", i);
    int has_rings = 0;
    for (int j = 0; j < MAX_RINGS; j++) {
      if (state[i][j] != 0) {
        printf("%d ", state[i][j]);
        has_rings = 1;
      }
    }
    if (!has_rings) {
      printf("[Empty]");
    }
    printf("\n");
  }
}

void print_solution(history_stack *solution) {
  printf("  Solution (%d moves): ", solution->top_idx + 1);
  if (solution->top_idx < 0) {
    printf("[No moves needed]\n");
    return;
  }
  for (int i = 0; i <= solution->top_idx; i++) {
    printf("(%d->%d) ", solution->moves[i].source, solution->moves[i].destination);
  }
  printf("\n");
}

void test_solver_from_initial_state(void) {
  printf("\n--- Test Case 1: Already at Initial State (All rings on Tower 0) ---\n");

  hanoi_init_game(3);
  history_stack solution;
  init_history(&solution);

  printf("Current state (already at start):\n");
  hanoi_print_game_state("Current", &current_game);

  optimal_solve(&solution);

  print_solution(&solution);

  // Already at goal (tower 0), should need 0 moves
  assert_equal_int(solution.top_idx + 1, 0, "TC1.1: Already at start position, should have 0 moves");

  printf("✅ Test Case 1 Complete\n");
}

void test_solver_from_partial_state_1(void) {
  printf("\n--- Test Case 2: Reset from Partial State (Rings scattered) ---\n");

  hanoi_init_game(3);

  // Manually set up state: Tower 0: [110], Tower 1: [], Tower 2: [60, 30]
  // Need to move everything back to tower 0
  for (int i = 0; i < NUM_TOWERS; i++) {
    init_stack(&current_game.towers[i]);
    memset(current_game.towers[i].rings, 0, sizeof(current_game.towers[i].rings));
  }

  push_tower(&current_game.towers[0], 110);
  push_tower(&current_game.towers[2], 60);
  push_tower(&current_game.towers[2], 30);

  printf("Starting state:\n");
  hanoi_print_game_state("Partial", &current_game);

  history_stack solution;
  init_history(&solution);

  optimal_solve(&solution);

  print_solution(&solution);

  // Should find a path back to tower 0
  int has_solution = solution.top_idx >= 0;
  assert_equal_int(has_solution, 1, "TC2.1: Should find solution to reset to tower 0");

  printf("✅ Test Case 2 Complete\n");
}

void test_solver_from_partial_state_2(void) {
  printf("\n--- Test Case 3: Reset from Mid-Game State ---\n");

  hanoi_init_game(3);

  // State: Tower 0: [110, 60], Tower 1: [30], Tower 2: []
  // Need to move ring 30 back to tower 0
  for (int i = 0; i < NUM_TOWERS; i++) {
    init_stack(&current_game.towers[i]);
    memset(current_game.towers[i].rings, 0, sizeof(current_game.towers[i].rings));
  }

  push_tower(&current_game.towers[0], 110);
  push_tower(&current_game.towers[0], 60);
  push_tower(&current_game.towers[1], 30);

  printf("Starting state:\n");
  hanoi_print_game_state("Mid-game", &current_game);

  history_stack solution;
  init_history(&solution);

  optimal_solve(&solution);

  print_solution(&solution);

  // Should be exactly 1 move: Tower 1 -> Tower 0
  assert_equal_int(solution.top_idx + 1, 1, "TC3.1: Should have exactly 1 move to reset");
  if (solution.top_idx >= 0) {
    assert_equal_int(solution.moves[0].source, 1, "TC3.2: Move should be from tower 1");
    assert_equal_int(solution.moves[0].destination, 0, "TC3.3: Move should be to tower 0");
  }

  printf("✅ Test Case 3 Complete\n");
}

void test_solver_already_solved(void) {
  printf("\n--- Test Case 4: Reset from Fully Moved State (All on Tower 2) ---\n");

  hanoi_init_game(3);

  // State: Tower 0: [], Tower 1: [], Tower 2: [110, 60, 30]
  // This is the "solved" state for forward Hanoi, but we need to reset to tower 0
  for (int i = 0; i < NUM_TOWERS; i++) {
    init_stack(&current_game.towers[i]);
    memset(current_game.towers[i].rings, 0, sizeof(current_game.towers[i].rings));
  }

  push_tower(&current_game.towers[2], 110);
  push_tower(&current_game.towers[2], 60);
  push_tower(&current_game.towers[2], 30);

  printf("Starting state (all on tower 2):\n");
  hanoi_print_game_state("All on Tower 2", &current_game);

  history_stack solution;
  init_history(&solution);

  optimal_solve(&solution);

  print_solution(&solution);

  // Should have 7 moves to reset back to tower 0
  assert_equal_int(solution.top_idx + 1, 7, "TC4.1: Should have 7 moves to reset from tower 2 to tower 0");

  printf("✅ Test Case 4 Complete\n");
}

void test_solver_one_move_away(void) {
  printf("\n--- Test Case 5: One Move Away from Reset ---\n");

  hanoi_init_game(3);

  // State: Tower 0: [110, 60], Tower 1: [30], Tower 2: []
  // Just need to move 30 from tower 1 back to tower 0
  for (int i = 0; i < NUM_TOWERS; i++) {
    init_stack(&current_game.towers[i]);
    memset(current_game.towers[i].rings, 0, sizeof(current_game.towers[i].rings));
  }

  push_tower(&current_game.towers[0], 110);
  push_tower(&current_game.towers[0], 60);
  push_tower(&current_game.towers[1], 30);

  printf("Starting state (one move away from reset):\n");
  hanoi_print_game_state("One Move", &current_game);

  history_stack solution;
  init_history(&solution);

  optimal_solve(&solution);

  print_solution(&solution);

  // Should have exactly 1 move
  assert_equal_int(solution.top_idx + 1, 1, "TC5.1: Should have exactly 1 move");

  // Verify the move is correct (1 -> 0)
  if (solution.top_idx >= 0) {
    assert_equal_int(solution.moves[0].source, 1, "TC5.2: Move source should be Tower 1");
    assert_equal_int(solution.moves[0].destination, 0, "TC5.3: Move destination should be Tower 0");
  }

  printf("✅ Test Case 5 Complete\n");
}

void test_solver_two_rings(void) {
  printf("\n--- Test Case 6: Two Rings Already at Start ---\n");

  hanoi_init_game(2);

  printf("Starting state (2 rings, already at start):\n");
  hanoi_print_game_state("Two Rings", &current_game);

  history_stack solution;
  init_history(&solution);

  optimal_solve(&solution);

  print_solution(&solution);

  // Already at goal, should be 0 moves
  assert_equal_int(solution.top_idx + 1, 0, "TC6.1: Already at start, should have 0 moves");

  printf("✅ Test Case 6 Complete\n");
}

void test_solver_single_ring(void) {
  printf("\n--- Test Case 7: Single Ring on Tower 2 ---\n");

  hanoi_init_game(1);

  // Move ring to tower 2, need to reset back to tower 0
  for (int i = 0; i < NUM_TOWERS; i++) {
    init_stack(&current_game.towers[i]);
    memset(current_game.towers[i].rings, 0, sizeof(current_game.towers[i].rings));
  }

  push_tower(&current_game.towers[2], 110);

  printf("Starting state (1 ring on tower 2):\n");
  hanoi_print_game_state("Single Ring", &current_game);

  history_stack solution;
  init_history(&solution);

  optimal_solve(&solution);

  print_solution(&solution);

  // Should find a solution to move back to tower 0
  int has_solution = solution.top_idx >= 0;
  assert_equal_int(has_solution, 1, "TC7.1: Should find solution to reset single ring");

  printf("✅ Test Case 7 Complete\n");
}

void test_get_moves_function(void) {
  printf("\n--- Test Case 8: get_moves() Function ---\n");

  // State: Tower 0: [110, 60], Tower 1: [30], Tower 2: []
  int state[NUM_TOWERS][MAX_RINGS] = {
    {110, 60, 0},
    {30, 0, 0},
    {0, 0, 0}
  };

  printf("State for get_moves:\n");
  print_state(state);

  int entry_idx = 0;
  Queue_Entry *possible_moves = get_moves(state, &entry_idx);

  printf("  Found %d possible moves\n", entry_idx);

  // Should have multiple valid moves from this state
  assert_equal_int(entry_idx > 0, 1, "TC8.1: Should have at least one valid move");

  // Print all possible moves
  for (int i = 0; i < entry_idx; i++) {
    printf("  Move %d: Tower %d -> Tower %d\n", i + 1,
           possible_moves[i].moves[0].source,
           possible_moves[i].moves[0].destination);
  }

  free(possible_moves);
  printf("✅ Test Case 8 Complete\n");
}

void test_find_top_of_state(void) {
  printf("\n--- Test Case 9: find_top_of_state() Function ---\n");

  int state1[MAX_RINGS] = {110, 60, 30};
  int state2[MAX_RINGS] = {110, 0, 0};
  int state3[MAX_RINGS] = {0, 0, 0};
  int state4[MAX_RINGS] = {0, 60, 30};

  int top1 = find_top_of_state(state1);
  int top2 = find_top_of_state(state2);
  int top3 = find_top_of_state(state3);
  int top4 = find_top_of_state(state4);

  assert_equal_int(top1, 2, "TC9.1: Full tower should have top at index 2");
  assert_equal_int(top2, 0, "TC9.2: Single ring should have top at index 0");
  assert_equal_int(top3, -1, "TC9.3: Empty tower should return -1");
  assert_equal_int(top4, 2, "TC9.4: Should find top ring at highest index");

  printf("✅ Test Case 9 Complete\n");
}

void test_states_are_equal(void) {
  printf("\n--- Test Case 10: states_are_equal() Function ---\n");

  int state1[NUM_TOWERS][MAX_RINGS] = {
    {110, 60, 30},
    {0, 0, 0},
    {0, 0, 0}
  };

  int state2[NUM_TOWERS][MAX_RINGS] = {
    {110, 60, 30},
    {0, 0, 0},
    {0, 0, 0}
  };

  int state3[NUM_TOWERS][MAX_RINGS] = {
    {110, 60, 0},
    {30, 0, 0},
    {0, 0, 0}
  };

  bool equal1 = states_are_equal(state1, state2);
  bool equal2 = states_are_equal(state1, state3);

  assert_equal_int(equal1, 1, "TC10.1: Identical states should be equal");
  assert_equal_int(equal2, 0, "TC10.2: Different states should not be equal");

  printf("✅ Test Case 10 Complete\n");
}

int main(void) {
  printf("=== Tower of Hanoi Solver Tests ===\n");

  test_find_top_of_state();
  test_states_are_equal();
  test_get_moves_function();
  test_solver_already_solved();
  test_solver_one_move_away();
  test_solver_single_ring();
  test_solver_two_rings();
  test_solver_from_initial_state();
  test_solver_from_partial_state_1();
  test_solver_from_partial_state_2();

  printf("\n=== All Solver Tests Complete ===\n");
  return 0;
}
