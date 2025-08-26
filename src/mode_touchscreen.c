#include "mode_touchscreen.h"
#include "nextion.h"


void clear_boxes(void) {
	nextion_send_command("c0.val=0");
	nextion_send_command("c0.val=0");
	nextion_send_command("c1.val=0");
	nextion_send_command("c1.val=0");
	nextion_send_command("c2.val=0");
	nextion_send_command("c2.val=0");
}

void update_txt_box(char * val) {
	write_to_txt_component(MAIN_TXT_BOX, val);
	
}