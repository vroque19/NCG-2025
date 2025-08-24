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