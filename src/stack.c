#include "stack.h"

void init_stack(tower_stack *tower) {
    if(tower==NULL) return;
    tower->top_idx = -1;
    tower->ring_count = 0;
    // TODO: total weight
}
uint8_t pop_tower(tower_stack *tower) {
    if(is_empty_tower(tower)) return -1;
    uint8_t ring_val = tower->rings[tower->top_idx];
    tower->top_idx--;
    tower->ring_count--;
    return ring_val;
}

int push_tower(tower_stack *tower, uint8_t ring_val) {
    if(is_full_tower(tower)) return -1;
    tower->rings[++tower->top_idx] = ring_val;
    tower->ring_count++;
    return 0;
}

bool is_empty_tower(tower_stack *tower) {
    return tower->top_idx == -1;
}

bool is_full_tower(tower_stack *tower) {
    return tower->top_idx == MAX_RINGS - 1;
}

uint8_t peek_tower(tower_stack *tower) {
    return tower->rings[tower->top_idx];
}