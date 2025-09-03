#include "mode_touchscreen.h"
#include "nextion.h"


void clear_boxes(void) {
	char *tower_btns[3] = {TOWER_0_BTN_ID, TOWER_1_BTN_ID, TOWER_2_BTN_ID};
	for(int i = 0; i < 3; i++) {
		write_to_btn_component(tower_btns[i], BUTTON_OFF);
	}
}

void select_box(int tower_idx) {
	printf("Selecting box %d \n", tower_idx);
	char *tower_btns[3] = {TOWER_0_BTN_ID, TOWER_1_BTN_ID, TOWER_2_BTN_ID};
	write_to_btn_component(tower_btns[tower_idx], BUTTON_ON);
}

void update_txt_box(char * val) {
	write_to_txt_component(MAIN_TXT_BOX, val);
	
}