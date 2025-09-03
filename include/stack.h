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

/**
 * @brief Initializes the stack
 Sets the top idx at -1. Sets ring count at 0.
 * @param tower A pointer to the tower stack.
 * @return void
 */
void init_stack(tower_stack *tower);
/**
 * @brief Pops the most recent move from the stack.
 * Decrements top_idx and copies the move to popped_move.
 * @param tower A pointer to the stack.
 * @return True if a move was popped successfully, false if the stack was empty.
 */
 uint8_t pop_tower(tower_stack *tower);

 /**
 * @brief Pushed to the stack.
 * Decrements top_idx and copies the move to popped_move.
 * @param tower A pointer to the history_stack.
 * @param ring_val A ring value to push to the stack.
 * @return True if a move was popped successfully, false if the stack was empty.
 */
 int push_tower(tower_stack *tower, uint8_t ring_val);
uint8_t peek_tower(tower_stack *tower);

bool is_empty_tower(tower_stack *tower);
bool is_full_tower(tower_stack *tower);
int get_top_idx_from_tower(tower_stack *tower);
uint8_t *get_rings_from_tower(tower_stack *tower);


#endif