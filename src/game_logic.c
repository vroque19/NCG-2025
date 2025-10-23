/*
MIT License

Copyright (c) 2025 Analog Devices

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "game_logic.h"
#include <stdio.h>
// #include <math.h>
#include "history.h"
#include "stack.h"
#include <string.h>
// Global game state
game_state_t current_game = {0};

char *txt_responses[5] = {
    "Move Made :)", "Cannot move larger\r\n ring on smaller ring",
    "No ring on start\r tower", "Deselected Tower", "Not valid tower"};

static const double ring_weights[] = {
    30,  // Size 1
    60,  // Size 2
    110, // Size 3
};
static void hanoi_init_game_tower_0(void);
void hanoi_init_game(uint8_t num_rings) {
  if (num_rings > MAX_RINGS)
    num_rings = MAX_RINGS;

  memset(&current_game, 0, sizeof(game_state_t));
  current_game.num_rings = num_rings;
  current_game.min_moves = (1 << num_rings) - 1; // 2^n - 1
  current_game.selected_tower = 255; // No tower selected initially (if we want
                                     // to have solve from any position)
  // Initialize towers
  for (int i = 0; i < NUM_TOWERS; i++) {
    current_game.towers[i].tower_id = i;
    init_stack(&current_game.towers[i]);
  }
  hanoi_init_game_tower_0();
  init_history(&current_game.move_history);
  printf("Hanoi game with %d rings (min moves: %d)\n", num_rings,
         current_game.min_moves);
}

static void hanoi_init_game_tower_0(void) {
  // tower_stack *tower0 = &current_game.towers[0];
  for (int i = 0; i < current_game.num_rings; i++) {
    push_tower(&current_game.towers[0], ring_weights[MAX_RINGS - 1 - i]);
  }
}

void hanoi_reset_game(void) { hanoi_init_game(current_game.num_rings); }

// robot will
bool hanoi_execute_move(uint8_t source_tower, uint8_t destination_tower) {
  move_tuple move;
  move.destination = destination_tower;
  move.source = source_tower;
  move_result_t result = hanoi_validate_move(source_tower, destination_tower);
  // write_to_txt_component(MAIN_TXT_BOX, txt_responses[result]);
  if (result != MOVE_VALID) {
    printf("Invalid move: %d -> %d (reason: %d)\n", source_tower,
           destination_tower, result);
    return false;
  }
  tower_stack *source = &current_game.towers[source_tower];
  tower_stack *dest = &current_game.towers[destination_tower];
  uint8_t moving_ring = pop_tower(source);
  push_tower(dest, moving_ring);
  push_history(&current_game.move_history, move);

  current_game.moves_made++;
  hanoi_print_game_state("Current Game State: ", &current_game);

  // Check win condition
  if (hanoi_is_solved()) {
    current_game.game_complete = true;
    // write_to_txt_component(MAIN_TXT_BOX, "GAME SOLVED <3");
    printf("🎉 Congratulations! Game complete in %d moves (minimum possible "
           "moves: %d)❤️❤️\n",
           current_game.moves_made, current_game.min_moves);
  }

  return true;
}

Queue_Entry *get_moves(int (*state)[3], int *entry_idx) {
  int poss_state[NUM_TOWERS][MAX_RINGS];
  move_tuple poss_moves;
  Queue_Entry *poss_entries = queue_entry_ctor(poss_state, poss_moves);
  for (int src = 0; src < NUM_TOWERS; src++) {
    for (int dst = 0; dst < MAX_RINGS; dst++) {
      // check for validity
      if (dst == src || state[src][0] == 0) {
        continue;
      }
      move_tuple new_move;
      new_move.source = src;
      new_move.destination = dst;
      int top_idx_src = find_top_of_state(state[src]);
      int top_idx_dst = find_top_of_state(state[dst]);

      if (top_idx_dst < 0 || state[top_idx_src] < state[top_idx_dst]) {
        int new_state[NUM_TOWERS][MAX_RINGS];
        for (int tower = 0; tower < NUM_TOWERS; tower++) {
          memcpy(new_state[tower], state[tower], sizeof(state[0]) * 3);
        }
        int to_add = state[src][top_idx_src];
        new_state[src][top_idx_src] = 0; // pop off src tower
        new_state[dst][top_idx_dst + 1] = to_add;
        poss_entries[*entry_idx].state = new_state;
        poss_entries[*entry_idx].moves[0] = new_move;
        *entry_idx += 1;
      }
    }
  }
  return poss_entries;
}
void optimal_solve(history_stack *solved_moves) {
  int goal_state[NUM_TOWERS][MAX_RINGS] = {
      {ring_weights[2], ring_weights[1], ring_weights[0]},
      {0, 0, 0},
      {0, 0, 0}};
  Queue *q;
  q = queue_init();
  Set *visited = set_init();
  while (!queue_empty(q)) {
    Queue_Entry entry = queue_pop(q);
    if (states_are_equal(goal_state, entry.state)) {
      for (int i = 0; i < q->rear; ++i) {
        push_history(solved_moves, q->entries->moves[i]);
      }
      int entry_idx = 0;
      Queue_Entry new_entry = get_moves(entry.state, &entry_idx);
      for (int i = 0; i < entry_idx; i++) {
        int new_state[NUM_TOWERS][MAX_RINGS];
        for (int i = 0; i < NUM_TOWERS; i++) {
          for (int j = 0; j < MAX_RINGS; j++) {
            new_state[i][j] = new_entry.state[i][j];
          }
        }
        move_tuple new_move;
        new_move.source = new_entry.moves[0].source;
        new_move.destination = new_entry.moves[0].destination;
        if (in_set(visited, new_state)) {
          continue;
        }
      }
    }
  }
}

move_result_t hanoi_validate_move(uint8_t source_tower,
                                  uint8_t destination_tower) {
  if (source_tower >= NUM_TOWERS || destination_tower >= NUM_TOWERS) {
    return MOVE_INVALID_PHYSICAL_MISMATCH;
  }
  if (source_tower == destination_tower) {
    return MOVE_INVALID_SAME_TOWER;
  }
  tower_stack *source = &current_game.towers[source_tower];
  tower_stack *dest = &current_game.towers[destination_tower];
  // Check if source tower has rings
  if (source->ring_count == 0) {
    return MOVE_INVALID_EMPTY_SOURCE;
  }
  // Get top ring from source
  uint8_t top_source = peek_tower(source);
  // If destination is empty, move is valid
  if (dest->ring_count == 0) {
    return MOVE_VALID;
  }
  // Get top ring from destination
  uint8_t top_dest = peek_tower(dest);

  // Check if trying to place larger ring on smaller
  if (top_source > top_dest) {
    return MOVE_INVALID_LARGER_ON_SMALLER;
  }

  return MOVE_VALID;
}

bool hanoi_is_solved() {
  tower_stack *target_tower = &current_game.towers[2];
  if (target_tower->ring_count != current_game.num_rings) {
    return false;
  }
  return true;
}

void hanoi_print_game_state(const char *name, game_state_t *game) {
  printf("  %s: Rings=%d, Moves=%d/%d, Complete=%s, Selected=%d\n", name,
         game->num_rings, game->moves_made, game->min_moves,
         game->game_complete ? "YES" : "NO", game->selected_tower);

  for (int t = 0; t < NUM_TOWERS; t++) {
    printf("    Tower %d (count=%d): ", t, game->towers[t].ring_count);
    if (game->towers[t].ring_count == 0) {
      printf("[Empty]");
    } else {
      for (int r = 0; r < game->towers[t].ring_count; r++) {
        printf("%d ", game->towers[t].rings[r]);
      }
    }
    printf("\n");
  }
}
