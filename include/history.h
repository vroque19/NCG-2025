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

#ifndef __HISTORY_H_
#define __HISTORY_H_

#include <stdint.h>
#include <stdbool.h>

#define MAX_MOVES 20 // Maximum number of moves to store in history

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
 Sets the top idx at -1
 * @param history A pointer to the history_stack.
 * @return void
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

/**
 * @brief Pushes new move to the history stack.
 * Increments top_idx and copies the move to pushed move.
 * @param history A pointer to the history_stack.
 * @param move A pointer to a move_tuple to store the pushed move.
 * @return 0 
 */
bool push_history(history_stack *history, move_tuple move);

/**
 * @brief Pops the most recent move from the history stack.
 * Decrements top_idx and copies the move to popped_move.
 * @param history A pointer to the history_stack.
 * @param popped_move A pointer to a move_tuple to store the popped move.
 * @return True if a move was popped successfully, false if the stack was empty.
 */
bool pop_history(history_stack *history, move_tuple *popped_move);

#endif