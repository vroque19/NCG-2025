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
	char name[] = STATUS_TXT;
	char prefix[] = ".txt=\"";
	char dest_buff[50]; // final command
	char suffix[] = "\"";
	snprintf(dest_buff, sizeof(dest_buff), "%s%s%s%s", name, prefix, status, suffix); // combine obj, pref, weight, suff into one commands
	for(int i = 0; i < 2; i++) {
		nextion_send_command(dest_buff);
	}
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
	MXC_Delay(MXC_DELAY_MSEC(50)); // wait for arm movement
	increment_count();
	touch_count = 0;
	update_txt_box("idle");
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