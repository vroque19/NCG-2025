#include "handlers.h"
#include "mode_touchscreen.h"

uint8_t move_count = 0;
uint8_t touch_count = 0;
int tower0[] = {4, 3, 2, 1};
int tower1[] = {0, 0, 0, 0};
int tower2[] = {0, 0, 0, 0};

// tower_stack tower0 = {
//     .rings = tower0,
//     .top_idx = 3
// };

// tower_stack tower1 = {
//     .rings = tower1,
//     .top_idx = -1
// };

// tower_stack tower2 = {
//     .rings = tower2,
//     .top_idx = -1
// };

// Update Move Count
void increment_count(void) {
    move_count++;
	char name[] = MOVE_COUNT_ID;
	char prefix[] = ".val=";
	char dest_buff[50]; // final command
	snprintf(dest_buff, sizeof(dest_buff), "%s%s%d%s", name, prefix, move_count); // combine obj, pref, weight, suff into one commands
    // printf("Move count : %d\n\n", move_count);
	for(int i = 0; i < 2; i++) {
		nextion_send_command(dest_buff);
	}
}
// Update Debug Status Txt Box
void udpate_status_txt(char *status) {
    move_count++;
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
    printf("exiting to main menu\n");
    move_count = 0;
}

void solenoid_handler(void) {
	udpate_status_txt("solenoid actuate\n");
	printf("solenoid function\n");
	return;
}

void handle_tower_0_btn(void) {
	if(touch_count == 0) {
		char val[] = "move from tower 0";
		update_txt_box(val);
		touch_count++;
        current_game.selected_tower = 0;
        return;
	} else {
		update_txt_box("moving to tower 0...");
		clear_boxes();
		MXC_Delay(MXC_DELAY_MSEC(50)); // wait for arm movement
        increment_count();
		touch_count = 0;
        update_txt_box("idle");

	}
}

void handle_tower_1_btn(void) {
	if(touch_count == 0) {
		char val[] = "move from tower 1 ";
		update_txt_box(val);
		touch_count++;
	} else {
		clear_boxes();
        update_txt_box("moving to tower 1...");
		MXC_Delay(MXC_DELAY_MSEC(50)); // wait for arm movement
        increment_count();
		touch_count = 0;
        update_txt_box("idle");
	}
}

void handle_tower_2_btn(void) {
	if(touch_count == 0) {
		char val[] = "move from tower 2";
		update_txt_box(val);
		touch_count++;
	} else {
		update_txt_box("moving to tower 2...");
        clear_boxes();
		MXC_Delay(MXC_DELAY_MSEC(50)); // wait for arm movement
        increment_count();
		touch_count = 0;
        update_txt_box("idle");
	}

}
