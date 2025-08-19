#include "mode_touchscreen.h"
#include "nextion.h"

volatile int UART_ISR_FLAG = 0;
int touch_count = 0;
int tower_prev = 0;
uint32_t base0, base1, base2;


int mode_touchscreen_run(void) {
	
	// ASYNC UART
	uint8_t rx_data[BYTES];
	uint8_t tx_data[BYTES];
	clear_boxes();
	// setup async uart transaction request
	mxc_uart_req_t read_req = {
		.uart = NEXTION_UART_REG,
		.txData = tx_data,
		.rxData = rx_data,
		.txLen = BYTES,
		.rxLen = BYTES,
		.txCnt = 0,
		.rxCnt = 0,
		.callback = readCallback
	}; 
	
	// uart interrupt init
	nextion_int_init();
	MXC_UART_TransactionAsync(&read_req); // rearm uart interrupt
	// nextion_send_command("t6.txt=\"no\"");
	base0 = calibrate(LOAD_CELL_0);
	base1 = calibrate(LOAD_CELL_1);
	base2 = calibrate(LOAD_CELL_2);
	poll_weights(base0, base1, base2);
	update_txt_box("begin");
	while(1) {
		while (!UART_ISR_FLAG) {
			
		}
		// print_buff(rx_data, BYTES);
		uint8_t page, component;
		page = get_page(rx_data);
		component = get_component(rx_data);
		printf("Page: %X | Component: %X\n", page, component);
		handle_touch_event(page, component);
		UART_ISR_FLAG = 0;
		poll_weights(base0, base1, base2);
		update_txt_box("Idle");

		MXC_UART_TransactionAsync(&read_req); // rearms interrupt
	}
	
}

/* finds the correct function for each touch event */
void handle_touch_event(uint8_t page_id, uint8_t comp_id) {
	const screen_component comp_table[] = {
		{PAGE1_ID, TOGGLE_1TO2_ID, handle_toggle_1to2_btn},
		{PAGE1_ID, TOWER_0_ID, handle_tower_0_btn},
		{PAGE1_ID, TOWER_1_ID, handle_tower_1_btn},
		{PAGE1_ID, TOWER_2_ID, handle_tower_2_btn},
	};
	for(int i = 0; i < sizeof(comp_table)/sizeof(screen_component); i++) {
		// printf("%d\n", page_id==comp_table[i].page);
		if(page_id==comp_table[i].page && comp_id==comp_table[i].component) {
			comp_table[i].handler_function();
			return;
		}
	}
}

void clear_boxes(void) {
	nextion_send_command("c0.val=0");
	nextion_send_command("c0.val=0");
	nextion_send_command("c1.val=0");
	nextion_send_command("c1.val=0");
	nextion_send_command("c2.val=0");
	nextion_send_command("c2.val=0");
}

void update_txt_box(char * val) {
	char name[] = "t6";
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
void handle_tower_0_btn(void) {
	if(touch_count == 0) {
		char val[] = "move from tower 0";
		update_txt_box(val);
		printf("1st tower: ");
		touch_count++;
	} else {
		clear_boxes();
		update_txt_box("moving to tower 0...");
		MXC_Delay(MXC_DELAY_MSEC(1000)); // wait for arm movement

		printf("2nd tower: ");
		touch_count = 0;
	}
	printf("tower 0, %d\n", touch_count);
	
	return;
}


void handle_tower_1_btn(void) {
	if(touch_count == 0) {
		char val[] = "move from tower 1 ";
		update_txt_box(val);
		printf("1st tower: ");
		touch_count++;
	} else {
		clear_boxes();

		update_txt_box("moving to tower 1...");
		MXC_Delay(MXC_DELAY_MSEC(1000)); // wait for arm movement

		printf("2nd tower: ");
		touch_count = 0;
	}
	printf("tower 1\n");
	
	// printf("Touch count : %d\n\n", touch_count);
	return;
}


void handle_tower_2_btn(void) {
	if(touch_count == 0) {
		char val[] = "move from tower 2";
		update_txt_box(val);
		printf("1st tower: ");
		touch_count++;
	} else {
		clear_boxes();
		update_txt_box("moving to tower 2...");
		MXC_Delay(MXC_DELAY_MSEC(1000)); // wait for arm movement

		printf("2nd tower: ");
		touch_count = 0;
	}
	printf("tower 2\n");
	
	// printf("Touch count : %d\n\n", touch_count);
	return;
}
void handle_toggle_1to2_btn(void) {
	update_txt_box("toggled to touchscreen mode");
	printf("toggling page\n");
	return;
}
