#include "handlers.h"
#include "global_uart_handler.h"
#include "mode_touchscreen.h"

static void switch_page_helper(page_t page, game_mode_t mode);

// uint8_t move_count = 0;
uint8_t touch_count = 0;

// Update Move Count
void increment_count(void) {
	char name[] = MOVE_COUNT_ID;
	char prefix[] = ".val=";
	char dest_buff[50]; // final command
	snprintf(dest_buff, sizeof(dest_buff), "%s%s%d%s", name, prefix, current_game.moves_made); // combine obj, pref, weight, suff into one commands
    // printf("Move count : %d\n\n", move_count);
	for(int i = 0; i < 2; i++) {
		nextion_send_command(dest_buff);
	}
}

// Update Debug Status Txt Box
void udpate_status_txt(char *status) {
	write_to_txt_component(STATUS_TXT, status);
}

void exit_to_main_menu(void) {
	hanoi_reset_game();
    printf("exiting to main menu. move count: %d\n", current_game.moves_made);
}

void solenoid_handler(void) {
	udpate_status_txt("solenoid actuate\n");
	printf("solenoid function\n");
	return;
}

void start_automated(void) {
	printf("Autosolving towers of hanoi in 15 moves\n\n\n");
	// TODO: add auto solving algorithm
}

// keep function local to this file
static void handle_tower_helper(int tower_idx) {
	char dest_buff[50];
	if(touch_count == 0) {
		sprintf(dest_buff, "move from tower %d", tower_idx);
		update_txt_box(dest_buff);
		touch_count++;
        current_game.selected_tower = tower_idx;
        return;
	}
	hanoi_execute_move(current_game.selected_tower, tower_idx);
	sprintf(dest_buff, "moving to tower  %d", tower_idx);
	update_txt_box(dest_buff);
	clear_boxes();
	poll_weights();
	MXC_Delay(MXC_DELAY_MSEC(50)); // wait for arm movement
	increment_count();
	nextion_write_game_state(&current_game);
	touch_count = 0;
	// update_txt_box("idle");
	
}
 // TODO: print tower states to the display
char *get_string_from_rings(int top_idx, uint8_t *tower_rings, char *tower_str, uint8_t str_size) {
	int offset = 0;
	// 1. write the opening bracket of the stack
	offset = snprintf(tower_str, str_size, "[");
	// 2. append each ring up to the top idx
	for(int i = 0; i <= top_idx; i++) {
			// Use snprintf to append the current number and a comma.
            // Pass the remaining buffer size and the current offset.
		offset += snprintf(tower_str + offset, str_size - offset, "%d  ", tower_rings[i]);
	}
	// 3. append the closing bracket
	snprintf(tower_str + offset, str_size - offset, "]");
	printf("\n");
	
}

void nextion_write_game_state(game_state_t *game) {
	char *txt_boxes_arr[3] = {"t9", "t11", "t10"};
	// for i in range 3
	// 		get_rings_from_tower
	// 		get_string_from_rings
	// 		write game state
	// test with tower 0 first
	// int top_idx = &game->towers[tower_idx].top_idx;
	for(int i = 0; i < 3; i++) {
		uint8_t *tower_rings = get_rings_from_tower(&game->towers[i]);
		int top_idx = get_top_idx_from_tower(&game->towers[i]);
		char rings_str[50];
		get_string_from_rings(top_idx, tower_rings, rings_str, sizeof(rings_str));
		write_to_txt_component(txt_boxes_arr[i], rings_str);
		// write to txt_comp

	}
	// printf("Rings buffer for tower 0: %s %d\n", rings_str, top_idx);
	
}

void handle_tower_0_btn(void) {
	handle_tower_helper(0);
}

void handle_tower_1_btn(void) {
	handle_tower_helper(1);
}

void handle_tower_2_btn(void) {
	handle_tower_helper(2);
}


static void switch_page_helper(page_t page, game_mode_t mode) {
	hanoi_init_game(MAX_RINGS);
	hanoi_print_game_state("Initialized game", &current_game);
	switch_mode(mode);
	MXC_Delay(1000);
	nextion_write_game_state(&current_game);

}

void switch_page_touchscreen(void) {
	// page_t page = PAGE_TOUCHSCREEN;
	printf("switching to touchscreen \n\n");

	// game_mode_t mode = TOUCHSCREEN_MODE;
	// switch_page_helper(page, mode);
	switch_page_helper(PAGE_TOUCHSCREEN, TOUCHSCREEN_MODE);
}
void switch_page_manual(void) {
	// page_t page = PAGE_TOUCHSCREEN;
	printf("switching to manual \n\n");

	// game_mode_t mode = TOUCHSCREEN_MODE;
	// switch_page_helper(page, mode);
	switch_page_helper(PAGE_MANUAL, MANUAL_MODE);
}
void switch_page_automated(void) {
	printf("switching to automated \n\n");
	// page_t page = PAGE_TOUCHSCREEN;
	// game_mode_t mode = TOUCHSCREEN_MODE;
	// switch_page_helper(page, mode);
	switch_page_helper(PAGE_AUTOMATED, AUTOMATED_MODE);
}


// Switch to a new operating mode
void switch_mode(game_mode_t new_mode) {
    if (new_mode == current_mode) return;
    
    // printf("Switching from %s mode to %s mode\n", 
    //        get_mode_name(current_mode), get_mode_name(new_mode));
    
    // Perform any cleanup for the old mode here
    current_mode = new_mode;

}