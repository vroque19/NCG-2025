#include "game_logic.h"
#include "history.h"
#include "stack.h"
#include <memory.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int state[NUM_TOWERS][MAX_RINGS];
  move_tuple moves[128];
  int moves_idx;
} Queue_Entry;

typedef struct {
  Queue_Entry entries[64];
  int front;
  int rear;
} Queue;

Queue_Entry *queue_entry_ctor(int (*state)[MAX_RINGS], move_tuple move) {
  Queue_Entry *mem = (Queue_Entry *)malloc(sizeof(Queue_Entry));
  mem->moves[mem->moves_idx].source = move.source;
  mem->moves[mem->moves_idx].destination = move.destination;
  for (int i = 0; i < NUM_TOWERS; ++i)
    for (int j = 0; j < MAX_RINGS; ++j)
      mem->state[i][j] = state[i][j];
  return mem;
}

Queue *queue_init(void) {
  Queue *mem = (Queue *)malloc(sizeof(Queue));
  memset(mem, 0, sizeof(Queue));
  mem->front = 0;
  mem->rear = 0;
  return mem;
}

void queue_push(Queue *queue, Queue_Entry entry) {
  queue->entries[queue->rear] = entry;
  queue->rear++;
}

bool queue_empty(Queue *queue) { return queue->front == queue->rear; }

Queue_Entry queue_peek(Queue *queue) { return queue->entries[queue->front]; }

Queue_Entry queue_pop(Queue *queue) {
  Queue_Entry entry = queue_peek(queue);
  queue->front++;
  return entry;
}

int find_top_of_state(int state[MAX_RINGS]) {
  for (int i = 2; i >= 0; i--) {
    if (state[i] != 0) {
      return i;
    }
  }
  return -1; // empty if -1
}
