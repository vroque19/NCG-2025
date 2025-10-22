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

#include "stack.h"

void init_stack(tower_stack *tower) {
    if(tower==NULL) return;
    tower->top_idx = -1;
    tower->ring_count = 0;
    tower->total_weight = 0;
}
uint8_t pop_tower(tower_stack *tower) {
    if(is_empty_tower(tower)) return -1;
    uint8_t ring_val = tower->rings[tower->top_idx];
  
    tower->total_weight = tower->total_weight - ring_val;
    tower->top_idx--;
    tower->ring_count--;
    return ring_val;
}

int push_tower(tower_stack *tower, uint8_t ring_val) {
    if(is_full_tower(tower)) return -1;
    tower->rings[++tower->top_idx] = ring_val;
    tower->ring_count++;
    tower->total_weight = tower->total_weight + ring_val;
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

uint8_t *get_rings_from_tower(tower_stack *tower) {
    return tower->rings;
}

int get_top_idx_from_tower(tower_stack *tower) {
    return tower->top_idx;
}
