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

#include "mode_touchscreen.h"
#include "nextion.h"
#include <stdio.h>
char *ring_comp_ids[3] = {RING_0, RING_1, RING_2};
uint8_t nextion_ring_pos_y[3] = {bottom_pos_y, mid_pos_y, top_pos_y};

// start positions for rings 0, 1, 2
uint8_t nextion_ring_pos_x[3] = {ring_0_pos_x, ring_1_pos_x, ring_2_pos_x};
// functions for display only used by touchscreen mode
static void move_ring_up(char *comp, int x1, int y1,  int x2, int y2, int priority, int time_ms);

static void move_ring_down(char *comp, int x1,int y1, int x2, int y2, int priority, int time_ms);

static void move_ring_across(char *comp, int x1, int y1, int x2, int y2, int priority, int time_ms);

static void move_ring_helper(char *comp, int x1, int y1, int x2, int y2, int priority, int time_ms);

void clear_boxes(void) {
	char *tower_btns[3] = {TOWER_0_BTN_ID, TOWER_1_BTN_ID, TOWER_2_BTN_ID};
	for(int i = 0; i < 3; i++) {
		write_to_btn_component(tower_btns[i], BUTTON_OFF);
	}
}

void select_box(int tower_idx) {
	// printf("Selecting box %d \n", tower_idx);
	char *tower_btns[3] = {TOWER_0_BTN_ID, TOWER_1_BTN_ID, TOWER_2_BTN_ID};
	write_to_btn_component(tower_btns[tower_idx], BUTTON_ON);
}

void update_txt_box(char * val) {
	write_to_txt_component(MAIN_TXT_BOX, val);
	
}


static void move_ring_up(char *comp, int x1, int y1,  int x2, int y2, int priority, int time_ms) {
	move_ring_helper( comp, x1, y1, x2, y2, priority, time_ms);
}

static void move_ring_down(char *comp, int x1,int y1, int x2, int y2, int priority, int time_ms) {
	move_ring_helper( comp, x1, y1, x2, y2, priority, time_ms);
}

static void move_ring_across(char *comp, int x1, int y1, int x2, int y2, int priority, int time_ms){
	move_ring_helper( comp, x1, y1, x2, y2, priority, time_ms);
}

// usage: move <comp>,<x1>,<y1>,<x2>,<y2>,<priority>,<time>
static void move_ring_helper(char *comp, int x1, int y1, int x2, int y2, int priority, int time_ms) {
	char dest_buff[64];
	snprintf(dest_buff, sizeof(dest_buff), "move %s,%d,%d,%d,%d,%d,%d", comp, x1, y1, x2, y2, priority, time_ms);
	nextion_send_command(dest_buff);
}

void nextion_move_rings(int source, int dest, int source_height, int dest_height, int ring_size) {
	// printf("\nMoving ring size %d across display\n from %d to %d\ns:%d d:%d\n\n", ring_size, source, dest, source_height, dest_height);
	__disable_irq(); // debounce

	char *comp = ring_comp_ids[ring_size];
	int x1 = nextion_ring_pos_x[ring_size] + source * tower_distance;
	int y1 = nextion_ring_pos_y[source_height];
	int x2 = x1;
	int y2 = 100; // hard-coded height for all rings to travel to
	// y pos changes
	// printf("Move up...%d, %d, %d, %d\n\n", x1, y1, x2, y2);
	move_ring_up(comp, x1, y1, x2, y2, 5, 700);
	y1=y2;
	x2 = nextion_ring_pos_x[ring_size] + dest * tower_distance;
	// x pos changes
	// printf("Move across...%d, %d, %d, %d\n\n", x1, y1, x2, y2);
	move_ring_across(comp, x1, y1, x2, y2, 4, 900);
	x1 = x2;
	y2 = nextion_ring_pos_y[dest_height];
	// y pos changes
	printf("Move down...%d, %d, %d, %d\n\n", x1, y1, x2, y2);

	move_ring_down(comp, x1, y1, x2, y2, 3, 500);

}

void nextion_change_ring_color(uint8_t ring_size, int color) {
	char dest_buff[64];
	char *ring_comp_id = ring_comp_ids[ring_size];
	// printf("Ring color changing: %s", ring_comp_id);
	snprintf(dest_buff, sizeof(dest_buff), "%s.bco=%d", ring_comp_id, color);
	nextion_send_command(dest_buff);
}