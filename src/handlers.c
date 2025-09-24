#include "handlers.h"
#include "TMC5272_SPI.h"
#include "game_logic.h"
#include "global_uart_handler.h"
#include "mode_touchscreen.h"
#include "motors.h"
#include "moves.h"
#include "mxc_delay.h"
#include "nextion.h"
#include "solenoid_driver.h"
#include "stack.h"

static void switch_page_helper(page_t page, game_mode_t mode);

// uint8_t move_count = 0;
uint8_t touch_count = 0;

// Update Move Count on display
void increment_count(void) {
	char name[] = MOVE_COUNT_ID;
	char prefix[] = ".val=";
	char dest_buff[50]; // final command
	snprintf(dest_buff, sizeof(dest_buff), "%s%s%d", name, prefix, current_game.moves_made); // combine obj, pref, weight, suff into one commands
    // printf("Move count : %d\n\n", move_count);
	for(int i = 0; i < 1; i++) {
		nextion_send_command(dest_buff);
	}
}

// Update Debug Status Txt Box
void udpate_status_txt(char *status) {
	write_to_txt_component(STATUS_TXT, status);
}

void exit_to_main_menu(void) {
	hanoi_reset_game();
	switch_mode(MENU);
	solenoid_off();
    printf("exiting to main menu. move count: %d\n", current_game.moves_made);
}

void solenoid_handler(void) {
	// tmc5272_dev_t *tmc_y = get_tmc_y_device();
	double *prev_weights = poll_weights();
	if(touch_count == 0) {
		/*
		Manual mode game logic
		1. get the prev weights 
		2/ actuate and move solenoid
		3. get curr weights 
		4. update game state
		*/
		
		solenoid_on();
		// tmc5272_rotateByMicrosteps(tmc_devices.tmc_y, ALL_MOTORS, 51200);
		// double *curr_weights = poll_weights();
		// for(int i = 0; i < 3; i++) {
		// 	current_game.selected_tower = -1;
		// 	if(prev_weights[i] - curr_weights[i] > 50) {
		// 		current_game.selected_tower = i;
		// 	}
		// }
		// if source < 0, no ring is picked up
		if(current_game.selected_tower < 0) {
			solenoid_off();
		}
		touch_count++;
	} else {
		solenoid_off();
		MXC_Delay(MXC_DELAY_MSEC(250)); // wait for ring to drop down
		double *curr_weights = poll_weights();
		update_weights(poll_weights());

		for(int i = 0; i < 3; i++) {
			current_game.selected_tower = -1;
			if(curr_weights[i] - prev_weights[i] > 50) {
				int dest_tower_idx = i;
					hanoi_execute_move(current_game.selected_tower, dest_tower_idx);

			}
		}
		touch_count = 0;
	}
	return;
}

// automated mode functions --
// recursive solution for auto solving algorithm
void auto_solve_hanoi(int num_rings, int source, int dest) {
	poll_weights();
	nextion_write_game_state(&current_game);
	increment_count();

	if(num_rings == 1) {
		hanoi_execute_move(source, dest);
	}
	else {
		int aux = 3 - (source + dest);
		auto_solve_hanoi(num_rings - 1, source, aux);
		hanoi_execute_move(source, dest);
		auto_solve_hanoi(num_rings - 1, aux, dest);
	}
	double* weights = poll_weights();
	update_weights(weights);
	nextion_write_game_state(&current_game);
	increment_count();
}

void start_automated(void) {
	printf("Autosolving towers of hanoi in 7 moves\n\n\n");
	auto_solve_hanoi(MAX_RINGS, 0, 2);
	if(current_game.game_complete) {
		printf("Game Complete :) %d\n", current_game.game_complete);
		return;
	}
}

// helper to handle touch-screen mode control
static void handle_tower_helper(int tower_idx) {
	if(current_game.is_busy) {
		return;
	}
	
	char dest_buff[50];
	MXC_Delay(MXC_DELAY_MSEC(250)); // wait for arm movement
	if(touch_count == 0) { // First tower selected
		sprintf(dest_buff, "move from tower %d", tower_idx);
		update_txt_box(dest_buff);
		touch_count++;
        current_game.selected_tower = tower_idx;
		uint8_t curr_tower = current_game.selected_tower;
		int source_height = get_top_idx_from_tower(&current_game.towers[curr_tower]);
		int selected_ring = peek_tower(&current_game.towers[tower_idx])-1;
		// only change color if valid tower selected
		if(source_height >= 0) {
			nextion_change_ring_color(selected_ring, RING_COLOR_SELECTED);
		}
        return;
	}
	current_game.is_busy = true;
	sprintf(dest_buff, "moving to tower  %d", tower_idx);
	update_txt_box(dest_buff);
	int source_tower = current_game.selected_tower;
	int dest_tower = tower_idx;
	int selected_ring = peek_tower(&current_game.towers[source_tower])-1;
	int source_height = get_top_idx_from_tower(&current_game.towers[source_tower]);
	int dest_height = get_top_idx_from_tower(&current_game.towers[dest_tower]) + 1;
	// animate rings on display
	move_result_t result = hanoi_validate_move(source_tower, tower_idx);
    write_to_txt_component(MAIN_TXT_BOX, txt_responses[result]);
	if((result)==MOVE_VALID) {
		nextion_move_rings(source_tower, dest_tower, source_height, dest_height, selected_ring);
		printf("Moved ring...");
		double * weights = poll_weights();
		update_weights(weights);
	}
	hanoi_execute_move(current_game.selected_tower, tower_idx);

	nextion_change_ring_color(selected_ring, RING_COLOR_DEFAULT);
	increment_count();
	// nextion_write_game_state(&current_game);
	touch_count = 0;

}


// could make a struct instead
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
	nextion_write_game_state(&current_game);
	MXC_Delay(5000);
	// poll_weights();
}

// New function to contain the continuous manual mode logic
void run_manual_mode_logic(tmc5272_dev_t *tmc_x, tmc5272_dev_t *tmc_y, tmc5272_dev_t *tmc_tc) {
		int32_t tc_x_pos = tmc5272_tricoder_getPosition(tmc_tc, TC_X);
		int32_t tc_y_pos = tmc5272_tricoder_getPosition(tmc_tc, TC_Y);
		
		// Rotate each axis to its encoder position
		tmc5272_rotateToPosition(tmc_x, MOTOR_0, 10*tc_x_pos);
		tmc5272_rotateToPosition(tmc_y, ALL_MOTORS, 10*tc_y_pos);

		// printf("Mx0: %d  ENC: %d", tmc5272_getPosition(tmc_x, MOTOR_0), tc_x_pos);
		// printf("\tMy0: %d  ENC: %d \n", tmc5272_getPosition(tmc_y, MOTOR_0), tc_y_pos);
}

void switch_page_touchscreen(void) {
	write_to_txt_component(MAIN_TXT_BOX, "Begin Solving \r\nTowers of Hanoi:)");
	printf("switching to touchscreen \n\n");
	switch_page_helper(PAGE_TOUCHSCREEN, TOUCHSCREEN_MODE);
}

// Prints the rings as a stack in string format
void get_string_from_rings(int top_idx, uint8_t *tower_rings, char *tower_str, uint8_t str_size) {
	int offset = 0;
	// 1. write the opening bracket of the stack
	offset = snprintf(tower_str, str_size, "[");
	// 2. append each ring up to the top idx
	for(int i = 0; i <= top_idx; i++) {
		offset += snprintf(tower_str + offset, str_size - offset, "%d  ", tower_rings[i]);
	}
	// 3. append the closing bracket
	snprintf(tower_str + offset, str_size - offset, "]");
	printf("\n");
}

// Writes current towers' states to the display
void nextion_write_game_state(game_state_t *game) {
	char *txt_boxes_arr[3] = {"t9", "t11", "t10"};
	for(int i = 0; i < 3; i++) {
		uint8_t *tower_rings = get_rings_from_tower(&game->towers[i]);
		int top_idx = get_top_idx_from_tower(&game->towers[i]);
		char rings_str[50];
		get_string_from_rings(top_idx, tower_rings, rings_str, sizeof(rings_str));
		write_to_txt_component(txt_boxes_arr[i], rings_str);
	}
	// printf("Rings buffer for tower 0: %s %d\n", rings_str, top_idx);
}

void switch_page_manual(void) {
	printf("switching to manual \n\n");
	switch_page_helper(PAGE_MANUAL, MANUAL_MODE);
	printf("Create Motor IC\n\n");
}


void switch_page_automated(void) {
	printf("switching to automated \n\n");
	switch_page_helper(PAGE_AUTOMATED, AUTOMATED_MODE);
}

// Switch to a new operating mode
void switch_mode(game_mode_t new_mode) {
	if (new_mode == current_mode) return;
    current_mode = new_mode;
	printf("Mode: %d\n", current_mode);

}