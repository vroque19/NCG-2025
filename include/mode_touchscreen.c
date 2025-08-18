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
  // uart interrupt init
  nextion_int_init();
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
	base0 = calibrate(LOAD_CELL_0);
	base1 = calibrate(LOAD_CELL_1);
	base2 = calibrate(LOAD_CELL_2);
	while(1) {
		while (!UART_ISR_FLAG) {
			// poll_weights(base0, base1, base2);
		  	MXC_UART_TransactionAsync(&read_req); // rearm uart interrupt
		}
		UART_ISR_FLAG = 0;
		print_buff(rx_data, BYTES);
		uint8_t page, component;
		page = get_page(rx_data);
		component = get_component(rx_data);
		printf("Page: %X | Component: %X\n", page, component);
		handle_touch_event(page, component);
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

void handle_tower_0_btn(void) {
	printf("tower 0\n");
	
	return;
}

void handle_tower_1_btn(void) {
	printf("tower 1\n");
	
	// printf("Touch count : %d\n\n", touch_count);
	return;
}
void handle_tower_2_btn(void) {
	printf("tower 2\n");
	
	// printf("Touch count : %d\n\n", touch_count);
	return;
}
void handle_toggle_1to2_btn(void) {
	printf("toggling page\n");
	return;
}
