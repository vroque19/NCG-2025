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

#include "history.h"
#include <stddef.h>
#include <stdbool.h>


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