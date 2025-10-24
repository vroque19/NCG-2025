#ifndef QUEUE_H
#define QUEUE_H

#include "game_logic.h"

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

// Function declarations (prototypes) only
Queue_Entry *queue_entry_ctor(int state[NUM_TOWERS][MAX_RINGS],
                              move_tuple move);
Queue *queue_init(void);
void queue_push(Queue *queue, Queue_Entry entry);
bool queue_empty(Queue *queue);
Queue_Entry *queue_peek(Queue *queue);
Queue_Entry *queue_pop(Queue *queue);
int find_top_of_state(int state[MAX_RINGS]);
#endif // QUEUE_H
