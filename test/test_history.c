#include "test_api.h"
#include "history.h"


void print_history_state(const char* name, history_stack *history) {
    printf("  %s (Moves: %d, Top Index: %d): ", name, 
           history->top_idx + 1, history->top_idx);
    if (is_history_empty(history)) {
        printf("[Empty]\n");
    } else {
        for (int i = 0; i <= history->top_idx; i++) {
            printf("(%d->%d) ", history->moves[i].start, history->moves[i].destination);
        }
        printf("\n");
    }
}
// Test Case 1: History initialization
void test_history_init(history_stack *history) {
    printf("--- Test Case 1: History Initialization ---\n");
    init_history(history);
    print_history_state("History Empty", history);
    assert_equal_int(history->top_idx, -1, "TC1.1: Top index is -1 after initialization");
    assert_true(is_history_empty(history), "TC1.2: History is empty after initialization");
    assert_false(is_history_full(history), "TC1.3: History is not full after initialization");
    print_history_state("test history", history);
}

// Test Case 2: History push (adding moves)
void test_history_push(history_stack *history) {
    printf("\n--- Test Case 2: Pushing moves to history ---\n");
    init_history(history);
    
    move_tuple move1 = {0, 1};  // Move from tower 0 to tower 1
    bool result1 = push_history(history, move1);
    assert_true(result1, "TC2.1: First push returns true");
    assert_equal_int(history->top_idx, 0, "TC2.2: Top index is 0 after first push");
    assert_equal_int(history->moves[0].start, 0, "TC2.3: First move start is correct");
    assert_equal_int(history->moves[0].destination, 1, "TC2.4: First move destination is correct");
    
    move_tuple move2 = {1, 2};  // Move from tower 1 to tower 2
    bool result2 = push_history(history, move2);
    assert_true(result2, "TC2.5: Second push returns true");
    assert_equal_int(history->top_idx, 1, "TC2.6: Top index is 1 after second push");
    assert_equal_int(history->moves[1].start, 1, "TC2.7: Second move start is correct");
    assert_equal_int(history->moves[1].destination, 2, "TC2.8: Second move destination is correct");
    
    move_tuple move3 = {2, 0};  // Move from tower 2 to tower 0
    bool result3 = push_history(history, move3);
    assert_true(result3, "TC2.9: Third push returns true");
    assert_equal_int(history->top_idx, 2, "TC2.10: Top index is 2 after third push");
    
    move_tuple move4 = {1, 1};
    bool result4 = push_history(history, move4);
    assert_true(result4, "TC2.10: Fourth push returns true");
    move_tuple move5 = {1, 1};
    move_tuple move6 = {1, 1};
    move_tuple move7 = {1, 1};
    push_history(history, move5);
    push_history(history, move6);
    push_history(history, move7);
    assert_equal_int(history->top_idx, 6, "TC2.11: Top index is 6 after move 7");
    
    print_history_state("test history", history);
}

// Test Case 3: History pop (removing moves, LIFO)
void test_history_pop(history_stack *history) {
    printf("\n--- Test Case 3: Popping moves from history (LIFO) ---\n");
    
    // Setup: push some moves first
    init_history(history);
    move_tuple move1 = {0, 1};
    move_tuple move2 = {1, 2};
    move_tuple move3 = {2, 0};
    push_history(history, move1);
    push_history(history, move2);
    push_history(history, move3);
    
    move_tuple popped_move;
    print_history_state("History for test pop", history);
    // Pop first move (should be the last one pushed - LIFO)
    bool result1 = pop_history(history, &popped_move);
    assert_true(result1, "TC3.1: First pop returns true");
    assert_equal_int(popped_move.start, 2, "TC3.2: Popped move $start is 2 (LIFO)");
    assert_equal_int(popped_move.destination, 0, "TC3.3: Popped move $destination is 0 (LIFO)");
    assert_equal_int(history->top_idx, 1, "TC3.4: Top index is 1 after first pop");
    print_history_state("test history", history);
    
    // Pop second move
    bool result2 = pop_history(history, &popped_move);
    assert_true(result2, "TC3.5: Second pop returns true");
    assert_equal_int(popped_move.start, 1, "TC3.6: Popped move $start is 1");
    assert_equal_int(popped_move.destination, 2, "TC3.7: Popped move $destination is 2");
    assert_equal_int(history->top_idx, 0, "TC3.8: Top index is 0 after second pop");
    print_history_state("test history", history);
    
    // Pop third move
    bool result3 = pop_history(history, &popped_move);
    assert_true(result3, "TC3.9: Third pop returns true");
    assert_equal_int(popped_move.start, 0, "TC3.10: Popped move $start is 0");
    assert_equal_int(popped_move.destination, 1, "TC3.11: Popped move $destination is 1");
    assert_true(is_history_empty(history), "TC3.12: History is empty after popping all moves");
    assert_equal_int(history->top_idx, -1, "TC3.13: Top index is -1 after popping all moves");
    print_history_state("test history", history);
}

// Test Case 4: History overflow
void test_history_overflow(history_stack *history) {
    printf("\n--- Test Case 4: History Overflow ---\n");
    init_history(history);
    
    // Fill the history to maximum capacity
    for (int i = 0; i < MAX_MOVES; i++) {
        print_history_state("test history for overflow", history);
        move_tuple move = {i % 3, (i + 1) % 3};  // Cycle through towers
        bool result = push_history(history, move);
        assert_true(result, "Push should succeed while under capacity");
    }
    
    assert_true(is_history_full(history), "TC4.1: History is full after pushing MAX_MOVES");
    assert_equal_int(history->top_idx, MAX_MOVES - 1, "TC4.2: Top index is MAX_MOVES - 1 when full");
    
    // Try to push one more move (should fail)
    move_tuple overflow_move = {0, 1};
    bool overflow_result = push_history(history, overflow_move);
    assert_false(overflow_result, "TC4.3: Pushing to full history returns false");
    assert_equal_int(history->top_idx, MAX_MOVES - 1, "TC4.4: Top index unchanged after failed push");
    
    print_history_state("test history", history);
}

// Test 5: History underflow
void test_history_underflow(history_stack *history) {
    printf("\n--- Test Case 5: History Underflow ---\n");
    init_history(history);
    print_history_state("History for underflow TC", history);
    move_tuple popped_move;
    bool result = pop_history(history, &popped_move);
    assert_false(result, "TC5.1 Popping from empty history returns false");
}

// Test Case 6: NULL pointer handling
void test_history_null_handling(void) {
    printf("\n--- Test Case 6: NULL Pointer Handling ---\n");
    
    // Test init_history with NULL
    init_history(NULL);
    assert_true(true, "TC6.1: init_history with NULL doesn't crash");
    
    // Test is_history_empty with NULL
    bool empty_result = is_history_empty(NULL);
    assert_true(empty_result, "TC6.2: is_history_empty with NULL returns true");
    
    // Test is_history_full with NULL
    bool full_result = is_history_full(NULL);
    assert_false(full_result, "TC6.3: is_history_full with NULL returns false");
    
    // Test push_history with NULL history
    move_tuple move = {0, 1};
    bool push_result1 = push_history(NULL, move);
    assert_false(push_result1, "TC6.4: push_history with NULL history returns false");
    
    // Test pop_history with NULL history
    move_tuple popped_move;
    bool pop_result1 = pop_history(NULL, &popped_move);
    assert_false(pop_result1, "TC6.5: pop_history with NULL history returns false");
    
    // Test pop_history with NULL move pointer
    history_stack history;
    init_history(&history);
    push_history(&history, move);
    bool pop_result2 = pop_history(&history, NULL);
    assert_false(pop_result2, "TC6.6: pop_history with NULL move pointer returns false");
}


// Test Case 9: Move data integrity
void test_history_move_integrity(history_stack *history) {
    printf("\n--- Test Case 9: Move Data Integrity ---\n");
    init_history(history);
    
    // Test various move combinations
    move_tuple moves[] = {
        {0, 2},   // Tower 0 to Tower 2
        {2, 1},   // Tower 2 to Tower 1
        {0, 1},   // Tower 0 to Tower 1
        {2, 0},   // Tower 2 to Tower 0
        {1, 0},   // Tower 1 to Tower 0
        {1, 2}    // Tower 1 to Tower 2
    };
    
    int num_moves = sizeof(moves) / sizeof(moves[0]);
    
    // Push all moves
    for (int i = 0; i < num_moves; i++) {
        bool result = push_history(history, moves[i]);
        assert_true(result, "Move push should succeed");
    }
    
    // Pop all moves and verify they come back in reverse order (LIFO)
    for (int i = num_moves - 1; i >= 0; i--) {
        move_tuple popped_move;
        bool result = pop_history(history, &popped_move);
        assert_true(result, "Move pop should succeed");
        assert_equal_int(popped_move.start, moves[i].start, 
                        "TC9.1: Popped move start matches expected (LIFO order)");
        assert_equal_int(popped_move.destination, moves[i].destination, 
                        "TC9.2: Popped move destination matches expected (LIFO order)");
    }
    
    assert_true(is_history_empty(history), "TC9.3: History is empty after popping all moves");
    print_history_state("test history", history);
}


int main(void) {
    history_stack game_history;
    test_history_init(&game_history);
    test_history_push(&game_history);
    test_history_pop(&game_history);
    test_history_overflow(&game_history);
    test_history_underflow(&game_history);
    test_history_null_handling();
    test_history_move_integrity(&game_history);
    // print_history_state("History", &game_history);
    return 0;
}