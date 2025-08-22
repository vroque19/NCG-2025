#include "stack.h"

void init_stack(tower_stack *tower) {
    if(tower->tower_id == 0) {
        for(int i = 0; i < MAX_RINGS; i++) {
            push_tower(&tower, i+1);
        }
        return;
    }
    tower->top_idx = -1;
    tower->ring_count = 0;
    // TODO: total weight
}
uint8_t pop_tower(tower_stack *tower) {
    if(is_empty_tower(&tower)) return;
    uint8_t ring_val = tower->rings[tower->top_idx];
    tower->top_idx--;
    tower->ring_count--;
    return ring_val;
}

void push_tower(tower_stack *tower, uint8_t *ring_val) {
    if(is_full_tower(tower)) return;
    tower->rings[++tower->top_idx] = *ring_val;
    tower->ring_count++;
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