#ifndef __STACK_H__
#define __STACK_H__
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#define MAX_RINGS 3

typedef struct{
    int top_idx;
    uint8_t tower_id;
    uint8_t rings[MAX_RINGS];
    uint8_t ring_count;
    double total_weight;
} tower_stack;

void init_stack(tower_stack *tower);
uint8_t pop_tower(tower_stack *tower);
uint8_t peek_tower(tower_stack *tower);
int push_tower(tower_stack *tower, uint8_t ring_val);
bool is_empty_tower(tower_stack *tower);
bool is_full_tower(tower_stack *tower);
int get_top_idx_from_tower(tower_stack *tower);
uint8_t *get_rings_from_tower(tower_stack *tower);


#endif