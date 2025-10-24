#include "game_logic.h"

#include "history.h"
#include "queue.h"
#include "set.h"
#include "stack.h"
#include "test_api.h"
#include <stdlib.h>
#include <string.h>

int print_move(move_tuple move) {
  printf("Source: %d\n Dest: %d\n", move.source, move.destination);
  return 0;
}

void print_state(int state[NUM_TOWERS][MAX_RINGS]) {
  for (int i = 0; i < NUM_TOWERS; i++) {
    printf("Tower %d\n", i);
    for (int j = 0; j < MAX_RINGS; j++) {
      printf("%d\n", state[i][j]);
    }
  }
}

int test_push(void) {
  Queue_Entry *entry;
  move_tuple move;
  move.destination = 1;
  move.source = 0;
  int tower_0[MAX_RINGS] = {110, 60, 30};
  int tower_1[MAX_RINGS];
  int tower_2[MAX_RINGS];
  int state[NUM_TOWERS][MAX_RINGS];
  memcpy(state[0], tower_0, sizeof(tower_0));
  memcpy(state[1], tower_1, sizeof(tower_1));
  memcpy(state[2], tower_2, sizeof(tower_2));
  entry = queue_entry_ctor(state, move);
  // print_move(move);
  // print_state(entry->state);

  Queue *queue;
  queue = queue_init();
  printf("queue fr: %d\t%d\n", queue->front, queue->rear);
  puts("e.state\n");
  print_state(entry->state);
  queue_push(queue, *entry);

  assert_equal_int(queue->rear, 1, "Expected 1 as rear idx\n");
  assert_equal_int(queue->entries[0].state[0][0], 110,
                   "Expected 110 as base ring\n");
  assert_equal_int(queue->entries[0].state[0][1], 60,
                   "Expected 60 as middle ring\n");
  print_state(queue->entries[0].state);
  free(queue);
  return 0;
}

void slightly_more_complicated_test(void) {
  Queue_Entry *entry1, *entry2;
  int state[NUM_TOWERS][MAX_RINGS] = {{110, 0, 0}, {60, 0, 0}, {30, 0, 0}};
  //  int *state[NUM_TOWERS] = {tower_0, tower_1, tower_2};
  move_tuple m;
  m.source = 0;
  m.destination = 1;
  entry1 = queue_entry_ctor(state, m);
  // int *state2[NUM_TOWERS] = {btower_0, btower_1, btower_2};
  int state2[NUM_TOWERS][MAX_RINGS] = {{110, 0, 0}, {60, 30, 0}, {0, 0, 0}};

  entry2 = queue_entry_ctor(state2, m);
  Queue *Q;
  Q = queue_init();
  queue_push(Q, *entry1);
  queue_push(Q, *entry2);
  printf("%d\n", Q->entries[Q->front].state[1][0]);
  assert_equal_int(Q->entries[0].state[1][1], 0,
                   "Expected 60 as middle ring\n");
  assert_equal_int(Q->entries[1].state[1][1], 30,
                   "Expected 60 as middle ring\n");
}

void set_add_test(void) {
  Set *set;
  set = set_init();
  int state[NUM_TOWERS][MAX_RINGS] = {{110, 0, 0}, {60, 0, 0}, {30, 0, 0}};
  assert_equal_int(set->idx, 0, "Should be 0 on init");
  set_add(set, state);
  assert_equal_int(set->items[0][0][0], 110, "Should be 110");
  assert_equal_int(set->idx, 1, "Should be 1 after add");
}

int main(void) {
  test_push();
  slightly_more_complicated_test();
  set_add_test();
  return 0;
}
