#include "game_logic.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "nextion.h"

// Global game state
game_state_t current_game = {0};

// TODO: Add weight thresholds 
static const double ring_weights[] = {
    1,  // Size 1
    2,  // Size 2
    3,  // Size 3
    // 50.0,  // Size 4
};
static void hanoi_init_game_tower_0(void);
void hanoi_init_game(uint8_t num_rings) {
    if (num_rings > MAX_RINGS) num_rings = MAX_RINGS;
    
    memset(&current_game, 0, sizeof(game_state_t));
    current_game.num_rings = num_rings;
    current_game.min_moves = (1 << num_rings) - 1; // 2^n - 1
    current_game.selected_tower = 255; // No tower selected initially (if we want to have solve from any position)
    // Initialize towers
    for (int i = 0; i < NUM_TOWERS; i++) {
        current_game.towers[i].tower_id = i;
        init_stack(&current_game.towers[i]);
    }
    hanoi_init_game_tower_0();
    init_history(&current_game.move_history);
    printf("Initialized Hanoi game with %d rings (min moves: %d)\n", 
           num_rings, current_game.min_moves);
}

static void hanoi_init_game_tower_0(void) {
    // tower_stack *tower0 = &current_game.towers[0];
    for(int i = 0; i < current_game.num_rings; i++) {
        push_tower(&current_game.towers[0], ring_weights[MAX_RINGS - 1 - i]);
    }
}

void hanoi_reset_game(void) {
    hanoi_init_game(current_game.num_rings);
}

// uint8_t hanoi_get_ring_size_from_weight(double weight) {
//     // Find the closest weight threshold
//     for (int i = 1; i <= MAX_RINGS; i++) {
//         if (weight <= ring_WEIGHT_THRESHOLDS[i] + 5.0) { // 5g tolerance
//             return i;
//         }
//     }
//     return 0; // No matching ring
// }

move_result_t hanoi_validate_move(uint8_t source_tower, uint8_t destination_tower) {
    move_tuple move;
    move.source = source_tower;
    move.destination = destination_tower;
    if (source_tower >= NUM_TOWERS || destination_tower >= NUM_TOWERS) {
        write_to_txt_component(MAIN_TXT_BOX, "Not valid tower");
        return MOVE_INVALID_PHYSICAL_MISMATCH;
    }
    if (source_tower == destination_tower) {
        write_to_txt_component(MAIN_TXT_BOX, "Deselected tower");
        return MOVE_INVALID_SAME_TOWER;
    }
    tower_stack *source = &current_game.towers[source_tower];
    tower_stack *dest = &current_game.towers[destination_tower];
    // Check if source tower has rings
    if (source->ring_count == 0) {
        write_to_txt_component(MAIN_TXT_BOX, "No ring on start\r tower");
        return MOVE_INVALID_EMPTY_SOURCE;
    }
    // Get top ring from source
    uint8_t top_source = peek_tower(source);
    // If destination is empty, move is valid
    if (dest->ring_count == 0) {
        write_to_txt_component(MAIN_TXT_BOX, "Move made :)");
        return MOVE_VALID;
    }
    // Get top ring from destination
    uint8_t top_dest = peek_tower(dest);
    
    // Check if trying to place larger ring on smaller
    if (top_source > top_dest) {
        write_to_txt_component(MAIN_TXT_BOX, "Cannot move larger\r\n ring on smaller ring");
        return MOVE_INVALID_LARGER_ON_SMALLER;
    }
    write_to_txt_component(MAIN_TXT_BOX, "Move made :)");

    return MOVE_VALID;
}

bool hanoi_execute_move(uint8_t source_tower, uint8_t destination_tower) {
    move_tuple move;
    move.destination = destination_tower;
    move.source = source_tower;
    move_result_t result = hanoi_validate_move(source_tower, destination_tower);
    
    if (result != MOVE_VALID) {
        printf("Invalid move: %d -> %d (reason: %d)\n", source_tower, destination_tower, result);
        return false;
    }
    
    tower_stack *source = &current_game.towers[source_tower];
    tower_stack *dest = &current_game.towers[destination_tower];
    
    // Move the ring
    /**
     * go to source()
     * pick up ring()
     * go to dest()
     * place ring()
     */
    uint8_t moving_ring = pop_tower(source);
    push_tower(dest, moving_ring);
    push_history(&current_game.move_history, move);

    current_game.moves_made++;
	hanoi_print_game_state("Initialized game", &current_game);

    
    // Check win condition
    if (hanoi_is_solved()) {
        current_game.game_complete = true;
        write_to_txt_component(MAIN_TXT_BOX, "GAME SOLVED <3");
        printf("🎉 Congratulations! Game complete in %d moves (min: %d)❤️❤️\n", 
               current_game.moves_made, current_game.min_moves);
    }
    
    return true;
}

bool hanoi_is_solved() {
    tower_stack *target_tower = &current_game.towers[2];
    if(target_tower->ring_count != current_game.num_rings) {
        return false;
    }
    return true;
}

void hanoi_print_game_state(const char* name, game_state_t *game) {
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