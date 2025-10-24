#include "game_logic.h"
#include "history.h"
#include "queue.h"
#include "set.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void debug_optimal_solve(history_stack *solved_moves) {
  int goal_state[NUM_TOWERS][MAX_RINGS] = {
      {0, 0, 0},
      {0, 0, 0},
      {110, 60, 30}};

  Queue *q = queue_init();
  Set *visited = set_init();

  Queue_Entry initial_entry;
  memset(&initial_entry, 0, sizeof(Queue_Entry));
  for (int i = 0; i < NUM_TOWERS; i++) {
    for (int j = 0; j < MAX_RINGS; j++) {
      initial_entry.state[i][j] = current_game.towers[i].rings[j];
    }
  }
  initial_entry.moves_idx = 0;

  printf("Initial state:\n");
  for (int i = 0; i < NUM_TOWERS; i++) {
    printf("  Tower %d: %d %d %d\n", i, 
      initial_entry.state[i][0], 
      initial_entry.state[i][1], 
      initial_entry.state[i][2]);
  }
  
  printf("Goal state:\n");
  for (int i = 0; i < NUM_TOWERS; i++) {
    printf("  Tower %d: %d %d %d\n", i, 
      goal_state[i][0], 
      goal_state[i][1], 
      goal_state[i][2]);
  }

  queue_push(q, initial_entry);
  set_add(visited, initial_entry.state);
  
  printf("\nStarting BFS...\n");
  int iterations = 0;

  while (!queue_empty(q)) {
    iterations++;
    Queue_Entry *current = queue_pop(q);
    
    printf("Iteration %d: front=%d rear=%d\n", iterations, q->front, q->rear);

    if (states_are_equal(goal_state, current->state)) {
      printf("GOAL FOUND at iteration %d! Move count: %d\n", iterations, current->moves_idx);
      for (int i = 0; i < current->moves_idx; i++) {
        push_history(solved_moves, current->moves[i]);
      }
      free(q);
      free(visited);
      return;
    }

    int num_moves = 0;
    Queue_Entry *possible_moves = get_moves(current->state, &num_moves);
    printf("  Generated %d possible moves\n", num_moves);

    for (int i = 0; i < num_moves; i++) {
      if (in_set(visited, possible_moves[i].state)) {
        printf("    Move %d: already visited\n", i);
        continue;
      }

      Queue_Entry next_entry = possible_moves[i];
      for (int j = 0; j < current->moves_idx && j < 127; j++) {
        next_entry.moves[j] = current->moves[j];
      }
      next_entry.moves[current->moves_idx] = possible_moves[i].moves[0];
      next_entry.moves_idx = current->moves_idx + 1;

      printf("    Move %d: %d->%d (new depth: %d)\n", i, 
        possible_moves[i].moves[0].source,
        possible_moves[i].moves[0].destination,
        next_entry.moves_idx);

      queue_push(q, next_entry);
      set_add(visited, next_entry.state);
    }

    free(possible_moves);
    
    if (iterations > 20) {
      printf("Breaking after 20 iterations for debug\n");
      break;
    }
  }

  printf("BFS ended without finding goal. Iterations: %d\n", iterations);
  free(q);
  free(visited);
}

int main(void) {
  printf("=== Debug Single Ring ===\n");
  hanoi_init_game(1);
  
  history_stack solution;
  init_history(&solution);
  
  debug_optimal_solve(&solution);
  
  printf("\nSolution has %d moves\n", solution.top_idx + 1);
  
  return 0;
}
