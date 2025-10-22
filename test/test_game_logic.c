#include "test_api.h"
#include "game_logic.h"
#include "history.h"


static const double ring_weights[] = {
    30,  // Size 1
    60,  // Size 2
    110,  // Size 3
};
void print_game_state(const char* name, game_state_t *game) {
    printf("  %s: Rings=%d, Moves=%d/%d, Complete=%s, Selected=%d\n", 
           name, game->num_rings, game->moves_made, game->min_moves,
           game->game_complete ? "YES" : "NO", game->selected_tower);
    
    for (int t = 0; t < NUM_TOWERS; t++) {
        printf("    Tower %d (count=%d): ", t, game->towers[t].ring_count);
        if (game->towers[t].ring_count == 0) {
            printf("[Empty]");
        } else {
            for (int r = 0; r < game->towers[t].ring_count; r++) {
                printf("%d ", game->towers[t].rings[r]);
            }
        }
        printf("\n");
    }
}

void print_history_state(const char* name, history_stack *history) {
    printf("  %s (Moves: %d, Top Index: %d): ", name, 
           history->top_idx + 1, history->top_idx);
    if (is_history_empty(history)) {
        printf("[Empty]\n");
    } else {
        for (int i = 0; i <= history->top_idx; i++) {
            printf("(%d->%d) ", history->moves[i].source, history->moves[i].destination);
        }
        printf("\n");
    }
}

// Test Case 1: Game initialization
void test_init_game(void) {
    printf("--- Test Case 1: Game Initialization ---\n");
    
    hanoi_init_game(4);
    
    assert_equal_uint8(current_game.num_rings, 4, "TC1.1: Number of rings set correctly");
    assert_equal_int(current_game.min_moves, 15, "TC1.2: Minimum moves calculated correctly (2^3-1=7)");
    assert_equal_int(current_game.moves_made, 0, "TC1.3: Moves made is 0 initially");
    assert_equal_uint8(current_game.selected_tower, 255, "TC1.4: No tower selected initially");
    assert_false(current_game.game_complete, "TC1.5: Game not complete initially");
    assert_false(current_game.is_busy, "TC1.6: Game not busy initially");
    
    // Check tower initialization
    for (int i = 0; i < NUM_TOWERS; i++) {
        assert_equal_uint8(current_game.towers[i].tower_id, i, "Tower ID set correctly");
        
    }
    
    assert_equal_int(current_game.towers[0].top_idx, 3, "TC1.7: Tower 0 properly initialized");
    assert_equal_int(current_game.towers[1].top_idx, -1, "TC1.8: Tower 1 properly initialized");
    assert_equal_uint8(current_game.towers[2].ring_count, 0, "TC1.9 tower 2 empty initially");
    print_game_state("initialized game", &current_game);
}

// Test Case 2: Game initialization with different ring counts
void test_game_init_various_sizes(void) {
    printf("\n--- Test Case 2: Game Initialization - Various Sizes ---\n");
    
    // Test with 1 ring
    hanoi_init_game(1);
    assert_equal_uint8(current_game.num_rings, 1, "TC2.1: 1 ring game initialized");
    assert_equal_int(current_game.min_moves, 1, "TC2.2: Min moves for 1 ring is 1");
    print_game_state("various size game", &current_game);
    
    // Test with 4 rings (max)
    hanoi_init_game(4);
    assert_equal_uint8(current_game.num_rings, 4, "TC2.3: 4 ring game initialized");
    assert_equal_int(current_game.min_moves, 15, "TC2.4: Min moves for 4 rings is 15");
    hanoi_init_game(3);
    assert_equal_uint8(current_game.num_rings, 3, "TC2.3: 3 ring game initialized");
    assert_equal_int(current_game.min_moves, 7, "TC2.4: Min moves for 3 rings is 7");
    print_game_state("3 rings", &current_game);
    
    // Test with excessive rings (should cap at MAX_RINGS)
    hanoi_init_game(10);
    assert_equal_uint8(current_game.num_rings, MAX_RINGS, "TC2.5: Excessive rings capped at MAX_RINGS");
    assert_equal_int(current_game.min_moves, (1 << MAX_RINGS) - 1, "TC2.6: Min moves calculated for MAX_RINGS");
    
    print_game_state("various size game", &current_game);
}

// Test Case 3: Game reset
void test_game_reset(void) {
    printf("\n--- Test Case 3: Game Reset ---\n");
    
    // Initialize and modify game state
    hanoi_init_game(3);
    current_game.moves_made = 5;
    current_game.selected_tower = 1;
    current_game.game_complete = true;
    print_game_state("Complete game game", &current_game);
    // Reset the game
    hanoi_reset_game();
    
    assert_equal_uint8(current_game.num_rings, 3, "TC3.1: Ring count preserved after reset");
    assert_equal_int(current_game.moves_made, 0, "TC3.2: Moves reset to 0");
    assert_equal_uint8(current_game.selected_tower, 255, "TC3.3: No tower selected after reset");
    assert_false(current_game.game_complete, "TC3.4: Game not complete after reset");
    print_history_state("\nHistory after reset move:", &current_game.move_history);
    
    print_game_state("reset game", &current_game);
}

// Test Case 4: Move validation - valid moves
void test_move_validation_valid(void) {
    printf("\n--- Test Case 4: Move Validation - Valid Moves ---\n");
    
    hanoi_init_game(3);
    
    // Setup: Put rings on towers manually for testing
    push_tower(&current_game.towers[0], ring_weights[2]);  // Large ring on tower 0
    push_tower(&current_game.towers[0], ring_weights[1]);  // Medium ring on tower 0
    push_tower(&current_game.towers[1], ring_weights[0]);  // Small ring on tower 1
    
    // Test valid moves
    move_result_t result1 = hanoi_validate_move(1, 0);  // Small ring (1) to medium ring (2)
    assert_equal_int(result1, MOVE_VALID, "TC4.1: Small ring on medium ring is valid");
    
    move_result_t result2 = hanoi_validate_move(1, 2);  // Small ring to empty tower
    assert_equal_int(result2, MOVE_VALID, "TC4.2: Any ring to empty tower is valid");
    
    print_game_state("validation test game", &current_game);
}

// Test Case 5: Move validation - invalid moves
void test_move_validation_invalid(void) {
    printf("\n--- Test Case 5: Move Validation - Invalid Moves ---\n");
    
    hanoi_init_game(3);
    // Setup towers for testing invalid moves
    for(int i = 0; i < NUM_TOWERS; i++) {
        pop_tower(&current_game.towers[0]);
    }
    push_tower(&current_game.towers[0], 3);  // Large ring on tower 0
    push_tower(&current_game.towers[1], 2);  // Medium ring on tower 1
    push_tower(&current_game.towers[1], 1);  // Small ring on tower 1
    print_game_state("\nsourceing state test game", &current_game);
    
    // Test invalid moves
    move_result_t result1 = hanoi_validate_move(0, 1);  // Large ring (3) on small ring (1)
    assert_equal_int(result1, MOVE_INVALID_LARGER_ON_SMALLER, "TC5.1: Large ring on small ring is invalid");
    
    move_result_t result2 = hanoi_validate_move(2, 1);  // From empty tower
    assert_equal_int(result2, MOVE_INVALID_EMPTY_SOURCE, "TC5.2: Move from empty tower is invalid");
    
    move_result_t result3 = hanoi_validate_move(1, 1);  // Same tower
    assert_equal_int(result3, MOVE_INVALID_SAME_TOWER, "TC5.3: Move to same tower is invalid");
    
    move_result_t result4 = hanoi_validate_move(5, 1);  // Invalid tower index
    assert_equal_int(result4, MOVE_INVALID_PHYSICAL_MISMATCH, "TC5.4: Invalid source tower index");
    
    move_result_t result5 = hanoi_validate_move(1, 5);  // Invalid tower index
    assert_equal_int(result5, MOVE_INVALID_PHYSICAL_MISMATCH, "TC5.5: Invalid destination tower index");
    
    print_game_state("invalid move test game unchanged", &current_game);
    
}

// Test Case 6: Move execution - successful moves
void test_move_execution_success(void) {
    printf("\n--- Test Case 6: Move Execution - Successful Moves ---\n");
    
    hanoi_init_game(2);
    
    // Setup: Put 2 rings on tower 0
    for(int i = 0; i < NUM_TOWERS; i++) {
        pop_tower(&current_game.towers[0]);
    }
    push_tower(&current_game.towers[0], 2);  // Large ring
    push_tower(&current_game.towers[0], 1);  // Small ring on top
    print_game_state("Test moving rings", &current_game);
    // Execute valid move: small ring from tower 0 to tower 1
    bool result1 = hanoi_execute_move(0, 1);
    assert_true(result1, "TC6.1: Valid move execution returns true");
    assert_equal_int(current_game.moves_made, 1, "TC6.2: Move counter incremented");
    assert_equal_uint8(current_game.towers[0].ring_count, 1, "TC6.3: Source tower ring count decreased");
    assert_equal_uint8(current_game.towers[1].ring_count, 1, "TC6.4: Destination tower ring count increased");
    assert_equal_uint8(peek_tower(&current_game.towers[1]), 1, "TC6.5: Correct ring moved to destination");
    assert_equal_uint8(peek_tower(&current_game.towers[0]), 2, "TC6.6: Correct ring remains on source");
    
    print_history_state("\nHistory after 1 move:", &current_game.move_history);
    print_game_state("\nState after successful move", &current_game);
}

// Test Case 7: Move execution - failed moves
void test_move_execution_failure(void) {
    printf("\n--- Test Case 7: Move Execution - Failed Moves ---\n");
    
    hanoi_init_game(2);
    
    // Setup: Put rings to create invalid move scenario
    for(int i = 0; i < NUM_TOWERS; i++) {
        pop_tower(&current_game.towers[0]);
    }
    push_tower(&current_game.towers[0], 2);  // Large ring on tower 0
    push_tower(&current_game.towers[1], 1);  // Small ring on tower 1
    
    int initial_moves = current_game.moves_made;
    
    // Try invalid move: large ring on small ring
    bool result = hanoi_execute_move(0, 1);
    assert_false(result, "TC7.1: Invalid move execution returns false");
    assert_equal_int(current_game.moves_made, initial_moves, "TC7.2: Move counter not incremented on invalid move");
    assert_equal_uint8(current_game.towers[0].ring_count, 1, "TC7.3: Source tower unchanged on invalid move");
    assert_equal_uint8(current_game.towers[1].ring_count, 1, "TC7.4: Destination tower unchanged on invalid move");
    print_history_state("\nHisotry after invalid move:", &current_game.move_history);
    print_game_state("\nState after failed move", &current_game);
}

// Test Case 8: Win condition detection
void test_win_condition(void) {
    printf("\n--- Test Case 8: Win Condition Detection ---\n");
    
    hanoi_init_game(2);
     for(int i = 0; i < NUM_TOWERS; i++) {
        pop_tower(&current_game.towers[0]);
    }
    print_game_state("Empty towers", &current_game);
    // Game not solved initially
    assert_false(hanoi_is_solved(), "TC8.1: Game not solved initially");
    assert_false(current_game.game_complete, "TC8.2: Game complete flag false initially");
    push_tower(&current_game.towers[0], 2);
    push_tower(&current_game.towers[0], 1);
    // Test actual game completion through execute_move
    hanoi_execute_move(0, 1);
    hanoi_execute_move(0, 2);
    // hanoi_execute_move(1, 2);
    
    bool move_result = hanoi_execute_move(1, 2);  // Move to final tower
    assert_true(hanoi_is_solved(), "TC8.3: Game detected as solved when all rings on tower 2");
    assert_true(move_result, "TC8.4: Final winning move executed successfully");
    assert_true(current_game.game_complete, "TC8.5: Game complete flag set after winning move");
    
    print_game_state("won game", &current_game);
}

// Test Case 9: Edge cases and boundary conditions
void test_edge_cases(void) {
    printf("\n--- Test Case 9: Edge Cases and Boundary Conditions ---\n");
    
    // Test with minimum rings (1)
    hanoi_init_game(1);
    assert_equal_uint8(current_game.num_rings, 1, "TC9.1: Single ring game initialized");
    assert_equal_int(current_game.min_moves, 1, "TC9.2: Single ring min moves is 1");
    
    // Test with maximum rings
    hanoi_init_game(MAX_RINGS);
    assert_equal_uint8(current_game.num_rings, MAX_RINGS, "TC9.3: Max rings game initialized");
    assert_equal_int(current_game.min_moves, (1 << MAX_RINGS) - 1, "TC9.4: Max rings min moves calculated correctly");
    
    // Test zero rings (edge case)
    hanoi_init_game(0);
    assert_equal_uint8(current_game.num_rings, 0, "TC9.5: Zero ring game handled");
    assert_equal_int(current_game.min_moves, 0, "TC9.6: Zero ring min moves is 0");
    
    print_game_state("edge case game", &current_game);
}

// Test Case 10: Tower state consistency
void test_tower_consistency(void) {
    printf("\n--- Test Case 10: Tower State Consistency ---\n");
    
    hanoi_init_game(3);
    
    // Verify all towers are properly initialized
    for (int i = 0; i < NUM_TOWERS; i++) {
        assert_equal_uint8(current_game.towers[i].tower_id, i, "Tower ID matches index");
        if (i == 0) {
            assert_equal_int(current_game.towers[i].top_idx, -1, "TC10.1: Tower 0 top_idx initialized");
        } else {
            assert_equal_int(current_game.towers[i].top_idx, 0, "TC10.2: Other towers top_idx set to 0");
        }
        assert_equal_uint8(current_game.towers[i].ring_count, 0, "TC10.3: All towers source empty");
    }
    
    print_game_state("consistency test game", &current_game);
}

// Test Case 11: Complete game sequence (2 rings)
void test_complete_game_sequence(void) {
    printf("\n--- Test Case 11: Complete Game Sequence (2 rings) ---\n");
    
    hanoi_init_game(2);
    for(int i = 0; i < 2; i++) {
        pop_tower(&current_game.towers[0]);
    }
    // Setup initial state: rings on tower 0
    push_tower(&current_game.towers[0], 2);  // Large ring
    push_tower(&current_game.towers[0], 1);  // Small ring on top
    
    // Optimal solution for 2 rings: 0->1, 0->2, 1->2 (3 moves)
    
    // Move 1: Small ring from tower 0 to tower 1
    bool move1 = hanoi_execute_move(0, 1);
    assert_true(move1, "TC11.1: Move 1 successful (0->1)");
    assert_equal_int(current_game.moves_made, 1, "TC11.2: Move count is 1");
    assert_false(current_game.game_complete, "TC11.3: Game not complete after move 1");
    
    // Move 2: Large ring from tower 0 to tower 2
    bool move2 = hanoi_execute_move(0, 2);
    assert_true(move2, "TC11.4: Move 2 successful (0->2)");
    assert_equal_int(current_game.moves_made, 2, "TC11.5: Move count is 2");
    assert_false(current_game.game_complete, "TC11.6: Game not complete after move 2");
    print_history_state("history after 2 moves", &current_game.move_history);
    // Move 3: Small ring from tower 1 to tower 2
    bool move3 = hanoi_execute_move(1, 2);
    assert_true(move3, "TC11.7: Move 3 successful (1->2)");
    assert_equal_int(current_game.moves_made, 3, "TC11.8: Move count is 3");
    assert_true(current_game.game_complete, "TC11.9: Game complete after final move");
    assert_true(hanoi_is_solved(), "TC11.10: Game detected as solved");
    
    // Verify final state
    assert_equal_uint8(current_game.towers[2].ring_count, 2, "TC11.11: All rings on final tower");
    assert_equal_uint8(current_game.towers[0].ring_count, 0, "TC11.12: Tower 0 empty");
    assert_equal_uint8(current_game.towers[1].ring_count, 0, "TC11.13: Tower 1 empty");
    
    print_game_state("completed game", &current_game);
}

// Test Case 12: Invalid move attempts
void test_invalid_move_attempts(void) {
    printf("\n--- Test Case 12: Invalid Move Attempts ---\n");
    
    hanoi_init_game(3);
    
    // Setup: rings on towers
    push_tower(&current_game.towers[0], 3);
    push_tower(&current_game.towers[0], 1);  // Small on large
    push_tower(&current_game.towers[1], 2);  // Medium ring alone
    
    int initial_moves = current_game.moves_made;
    
    // Try to move large ring onto small ring
    bool result1 = hanoi_execute_move(0, 1);  // Ring 1 to ring 2 (should fail validation)
    assert_false(result1, "TC12.1: Invalid move rejected (large on small)");
    assert_equal_int(current_game.moves_made, initial_moves, "TC12.2: Move count unchanged after invalid move");
    
    // Try to move from empty tower
    bool result2 = hanoi_execute_move(2, 0);  // From empty tower 2
    assert_false(result2, "TC12.3: Move from empty tower rejected");
    
    // Try to move to same tower
    bool result3 = hanoi_execute_move(0, 0);  // Same tower
    assert_false(result3, "TC12.4: Move to same tower rejected");
    
    print_game_state("invalid move test game", &current_game);
}

// Test Case 13: Win condition edge cases
void test_win_condition_edge_cases(void) {
    printf("\n--- Test Case 13: Win Condition Edge Cases ---\n");
    
    // Test with no rings
    hanoi_init_game(0);
    assert_true(hanoi_is_solved(), "TC13.1: Zero ring game is solved by default");
    
    // Test with single ring not on final tower
    hanoi_init_game(1);
    push_tower(&current_game.towers[0], 1);
    assert_false(hanoi_is_solved(), "TC13.2: Single ring on wrong tower not solved");
    
    // Move single ring to final tower
    pop_tower(&current_game.towers[0]);
    push_tower(&current_game.towers[2], 1);
    assert_true(hanoi_is_solved(), "TC13.3: Single ring on final tower is solved");
    
    // Test partial solution (not all rings on final tower)
    hanoi_init_game(3);
    push_tower(&current_game.towers[2], 3);  // Only large ring on final tower
    push_tower(&current_game.towers[1], 2);  // Medium ring elsewhere
    push_tower(&current_game.towers[1], 1);  // Small ring elsewhere
    assert_false(hanoi_is_solved(), "TC13.4: Partial solution not detected as solved");
    
    print_game_state("edge case game", &current_game);
}

// Test Case 14: Move validation return codes
void test_move_validation_return_codes(void) {
    printf("\n--- Test Case 14: Move Validation Return Codes ---\n");
    
    hanoi_init_game(2);
    push_tower(&current_game.towers[0], 2);
    push_tower(&current_game.towers[1], 1);
    
    // Test all possible validation results
    move_result_t result1 = hanoi_validate_move(0, 1);
    assert_equal_int(result1, MOVE_INVALID_LARGER_ON_SMALLER, "TC14.1: Larger on smaller detected");
    
    move_result_t result2 = hanoi_validate_move(2, 1);
    assert_equal_int(result2, MOVE_INVALID_EMPTY_SOURCE, "TC14.2: Empty source detected");
    
    move_result_t result3 = hanoi_validate_move(1, 1);
    assert_equal_int(result3, MOVE_INVALID_SAME_TOWER, "TC14.3: Same tower detected");
    
    move_result_t result4 = hanoi_validate_move(1, 2);
    assert_equal_int(result4, MOVE_VALID, "TC14.4: Valid move to empty tower");
    
    move_result_t result5 = hanoi_validate_move(3, 1);
    assert_equal_int(result5, MOVE_INVALID_PHYSICAL_MISMATCH, "TC14.5: Physical mismatch detected");
    
    print_game_state("validation codes test", &current_game);
}

// Test Case 15: Game state preservation during invalid moves
void test_state_preservation(void) {
    printf("\n--- Test Case 15: State Preservation During Invalid Moves ---\n");
    
    hanoi_init_game(3);
    
    // Setup initial state
    push_tower(&current_game.towers[0], 3);
    push_tower(&current_game.towers[0], 2);
    push_tower(&current_game.towers[0], 1);
    
    // Capture initial state
    uint8_t initial_tower0_count = current_game.towers[0].ring_count;
    uint8_t initial_tower1_count = current_game.towers[1].ring_count;
    uint8_t initial_tower2_count = current_game.towers[2].ring_count;
    uint8_t initial_top_ring = peek_tower(&current_game.towers[0]);
    int initial_moves = current_game.moves_made;
    
    // Attempt several invalid moves
    hanoi_execute_move(0, 0);  // Same tower
    hanoi_execute_move(1, 2);  // From empty tower
    
    // Verify state unchanged
    assert_equal_uint8(current_game.towers[0].ring_count, initial_tower0_count, "TC15.1: Tower 0 count preserved");
    assert_equal_uint8(current_game.towers[1].ring_count, initial_tower1_count, "TC15.2: Tower 1 count preserved");
    assert_equal_uint8(current_game.towers[2].ring_count, initial_tower2_count, "TC15.3: Tower 2 count preserved");
    assert_equal_uint8(peek_tower(&current_game.towers[0]), initial_top_ring, "TC15.4: Top ring preserved");
    assert_equal_int(current_game.moves_made, initial_moves, "TC15.5: Move count preserved");
    
    print_game_state("state preservation test", &current_game);
}

void test_weight_logic(void) {
  
    printf("\n--- Test Case 16: checking if weight are accurate --\n");
    
    hanoi_init_game(3);
    
    // Setup initial state
    push_tower(&current_game.towers[0], ring_weights[2]);
    push_tower(&current_game.towers[0], ring_weights[1]);
    push_tower(&current_game.towers[0], ring_weights[0]);
    
    hanoi_execute_move(0, 2);
    hanoi_execute_move(2, 1);
    hanoi_execute_move(0, 2);
    assert_equal_int(current_game.towers[0].total_weight, ring_weights[2], "TC16.1 Total Weight of 110 g expected/n");
    assert_equal_int(current_game.towers[1].total_weight, ring_weights[0], "TC16.2 Total Weight of 60 g expected/n");
    assert_equal_int(current_game.towers[2].total_weight, ring_weights[1], "TC16.3 Total Weight of 30g expected");
    print_game_state("Weight Test", &current_game);
}


int main(void) {
    printf("=== Game Logic Module Tests ===\n");
    test_init_game();
    test_game_init_various_sizes();
    test_game_reset();
    test_move_validation_valid();
    test_move_validation_invalid();
    test_move_execution_failure();
    test_win_condition();
    test_complete_game_sequence();
    test_win_condition_edge_cases();
    test_edge_cases();
    test_move_validation_return_codes();
    test_state_preservation();
    test_weight_logic();
    printf("\n=== Game Logic Tests Complete ===\n");
    return 0;
}
