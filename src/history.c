#include "history.h"
#include <stddef.h>
#include <stdbool.h> // For bool type

void init_history(history_stack *history) {
    if (history == NULL) {
        // Handle error: history pointer is NULL
        return;
    }
    history->top_idx = -1; // Initialize to -1 for an empty stack
}


bool is_history_empty(const history_stack *history) {
    if (history == NULL) {
        // Handle error: history pointer is NULL, consider it empty or an error
        return true;
    }
    return history->top_idx == -1;
}


bool is_history_full(const history_stack *history) {
    if (history == NULL) {
        // Handle error: history pointer is NULL, consider it not full or an error
        return false;
    }
    return history->top_idx == MAX_MOVES - 1;
}


bool push_history(history_stack *history, move_tuple move) {
    if (history == NULL) {
        // Handle error: history pointer is NULL
        return false;
    }
    if (is_history_full(history)) {
        // History is full, cannot add more moves
        return false;
    }
    history->top_idx++;
    history->moves[history->top_idx] = move; // Add the move to the top
    return true;
}

/**
 * @brief Pops the most recent move from the history stack.
 * Decrements top_idx and copies the move to popped_move.
 * @param history A pointer to the history_stack.
 * @param popped_move A pointer to a move_tuple to store the popped move.
 * @return True if a move was popped successfully, false if the stack was empty.
 */
bool pop_history(history_stack *history, move_tuple *popped_move) {
    if (history == NULL || popped_move == NULL) {
        // Handle error: history or popped_move pointer is NULL
        return false;
    }
    if (is_history_empty(history)) {
        // History is empty, no moves to pop
        return false;
    }
    *popped_move = history->moves[history->top_idx]; // Get the top move
    history->top_idx--; // Decrement the top index
    return true;
}