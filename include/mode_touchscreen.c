#include "mode_touchscreen.h"
#include "nextion.h"

// volatile int UART_ISR_FLAG = 0;
// int touch_count = 0;
// int tower_prev = 0;
uint32_t base0, base1, base2;

void clear_boxes(void) {
	nextion_send_command("c0.val=0");
	nextion_send_command("c0.val=0");
	nextion_send_command("c1.val=0");
	nextion_send_command("c1.val=0");
	nextion_send_command("c2.val=0");
	nextion_send_command("c2.val=0");
}

void update_txt_box(char * val) {
	char name[] = "t8";
	char prefix[] = ".txt=\"";
	char dest_buff[50]; // final command
	char suffix[] = "\"";
	snprintf(dest_buff, sizeof(dest_buff), "%s%s%s%s", name, prefix, val, suffix); // combine obj, pref, weight, suff into one commands
	// printf("Combined command: %s\n", dest_buff); // debugging what is being sent
	for(int i = 0; i < 2; i++) {
		nextion_send_command(dest_buff);
	
	}
	nextion_send_command(dest_buff);
}
// void handle_tower_0_btn(void) {
// 	if(touch_count == 0) {
// 		char val[] = "move from tower 0";
// 		update_txt_box(val);
// 		printf("1st tower: ");
// 		touch_count++;
// 	} else {
// 		clear_boxes();
// 		update_txt_box("moving to tower 0...");
// 		MXC_Delay(MXC_DELAY_MSEC(1000)); // wait for arm movement

// 		printf("2nd tower: ");
// 		touch_count = 0;
// 	}
// 	printf("tower 0, %d\n", touch_count);
	
// 	return;
// }


// void handle_tower_1_btn(void) {
// 	if(touch_count == 0) {
// 		char val[] = "move from tower 1 ";
// 		update_txt_box(val);
// 		printf("1st tower: ");
// 		touch_count++;
// 	} else {
// 		clear_boxes();

// 		update_txt_box("moving to tower 1...");
// 		MXC_Delay(MXC_DELAY_MSEC(1000)); // wait for arm movement

// 		printf("2nd tower: ");
// 		touch_count = 0;
// 	}
// 	printf("tower 1\n");
	
// 	// printf("Touch count : %d\n\n", touch_count);
// 	return;
// }


// void handle_tower_2_btn(void) {
// 	if(touch_count == 0) {
// 		char val[] = "move from tower 2";
// 		update_txt_box(val);
// 		printf("1st tower: ");
// 		touch_count++;
// 	} else {
// 		clear_boxes();
// 		update_txt_box("moving to tower 2...");
// 		MXC_Delay(MXC_DELAY_MSEC(1000)); // wait for arm movement

// 		printf("2nd tower: ");
// 		touch_count = 0;
// 	}
// 	printf("tower 2\n");
	
// 	// printf("Touch count : %d\n\n", touch_count);
// 	return;
// }

