
#include "moves.h"

char* txt_responses[5] = {
    "Move Made :)",
    "Cannot move larger\r\n ring on smaller ring",
    "No ring on start\r tower",
    "Deselected Tower",
    "Not valid tower"};

void hanoi_execute_move(uint8_t source_tower, uint8_t destination_tower) {
    move_tuple move;
    move.destination = destination_tower;
    move.source = source_tower;
    move_result_t result = hanoi_validate_move(source_tower, destination_tower);
    write_to_txt_component(MAIN_TXT_BOX, txt_responses[result]);
    if (result != MOVE_VALID) {
        printf("Invalid move: %d -> %d (reason: %d)\n", source_tower, destination_tower, result);
        return;
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
        printf("🎉 Congratulations! Game complete in %d moves (minimum possible moves: %d)❤️❤️\n", 
               current_game.moves_made, current_game.min_moves);
    }
    
    return;
}
