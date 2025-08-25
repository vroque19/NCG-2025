#ifndef __HISTORY_H_
#define __HISTORY_H_

#include <stdint.h>
#include <stdbool.h>

#define MAX_MOVES 32 // Maximum number of moves to store in history

// Structure to represent a single move
typedef struct {
    int source;         // The starting position of the move
    int destination;   // The destination position of the move
} move_tuple;

// Structure to manage the history of moves as a stack
typedef struct {
    move_tuple moves[MAX_MOVES]; // Array to store move_tuples
    int top_idx;                 // Index of the top element in the stack (-1 for empty)
} history_stack;

// Function prototypes for history management

/**
 * @brief Initializes the history stack.
 * @param history A pointer to the history_stack to initialize.
 */
void init_history(history_stack *history);

/**
 * @brief Checks if the history stack is empty.
 * @param history A pointer to the history_stack.
 * @return True if the history is empty, false otherwise.
 */
bool is_history_empty(const history_stack *history);

/**
 * @brief Checks if the history stack is full.
 * @param history A pointer to the history_stack.
 * @return True if the history is full, false otherwise.
 */
bool is_history_full(const history_stack *history);

bool push_history(history_stack *history, move_tuple move);


bool pop_history(history_stack *history, move_tuple *popped_move);

#endif