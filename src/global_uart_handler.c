// // global_uart_handler.c
#include "global_uart_handler.h"
#include "handlers.h"
#include "nextion.h"
#include "game_logic.h"

// Global state variables
volatile game_mode_t current_mode = TOUCHSCREEN_MODE;
volatile int GLOBAL_UART_ISR_FLAG = 0;
volatile page_t current_page = PAGE_MAIN_MENU;

// Static variables for UART management
static uint8_t global_rx_buffer[BYTES];
static uint8_t global_tx_buffer[BYTES];
static mxc_uart_req_t global_uart_req;

extern const screen_component comp_table[] = {
        /* TODO: add all components */
		{PAGE_TOUCHSCREEN, TOWER_0_ID, handle_tower_0_btn},
		{PAGE_TOUCHSCREEN, TOWER_1_ID, handle_tower_1_btn},
		{PAGE_TOUCHSCREEN, TOWER_2_ID, handle_tower_2_btn},
		{PAGE_TOUCHSCREEN, EXIT_TOUCHSCREEN_ID, exit_to_main_menu},
		{PAGE_MANUAL, SOLENOID_ID, solenoid_handler},
        {PAGE_MANUAL, EXIT_MANUAL_ID, exit_to_main_menu},
        {PAGE_AUTOMATED, START_AUTOMATED_ID, start_automated},
        {PAGE_MAIN_MENU, PAGE_TOUCHSCREEN_ID, switch_page_touchscreen},
        {PAGE_MAIN_MENU, PAGE_MANUAL_ID, switch_page_manual},
        {PAGE_MAIN_MENU, PAGE_AUTOMATED_ID, switch_page_automated}
};


void global_uart_interrupt_enable(void) {
    NVIC_ClearPendingIRQ(UART1_IRQn);
    NVIC_DisableIRQ(UART1_IRQn);
    MXC_NVIC_SetVector(UART1_IRQn, uart_isr);
    NVIC_EnableIRQ(UART1_IRQn);
    MXC_UART_SetRXThreshold(GLOBAL_UART_REG, BYTES);
    MXC_UART_EnableInt(GLOBAL_UART_REG, RX_LVL); 
}

/* ISR for the UART interrupts to set flags*/
void uart_isr(void) {
    // printf("\n\n~~~~~~ In ISR. Flag = %d ~~~~~~\n", GLOBAL_UART_ISR_FLAG);
    unsigned int flags = MXC_UART_GetFlags(GLOBAL_UART_REG);
    // printf("Flags: %d \n", flags);
    // __disable_irq();
    // printf("Flags: %d \n", flags);
    // Only process for RX Threshold interrupt
    if (( flags & RX_LVL) && (GLOBAL_UART_ISR_FLAG == 0)) {
        printf("Processing RX threshold interrupt :))\n");
        GLOBAL_UART_ISR_FLAG = 1;
        MXC_UART_AsyncHandler(GLOBAL_UART_REG);
    }
    else {
        // printf("------Ignoring interrupt - flags=0x%X, ISR_FLAG=%d\n-----",  flags, GLOBAL_UART_ISR_FLAG);
    }
    MXC_UART_ClearFlags(GLOBAL_UART_REG,  flags & ~RX_LVL);
    MXC_UART_EnableInt(GLOBAL_UART_REG, RX_LVL);
    MXC_UART_ClearRXFIFO(GLOBAL_UART_REG);
    // __enable_irq();
}

void global_uart_callback(mxc_uart_req_t *req, int error) {
    /* Called once per successful transaction */
    /* Possible reason for display error */
    printf(">>>> Callback function <<<< \n\n");
    if (error == E_NO_ERROR) {
        GLOBAL_UART_ISR_FLAG = 1;
        
        printf("UART transaction completed successfully\n");
        printf("Received %d bytes\n", req->rxCnt);
        printf("Raw data: ");
        print_buff(global_rx_buffer, req->rxCnt);
        printf("\n");
        
    } else {
        // Handle errors
        printf("UART transaction error: %d\n", error);
        switch(error) {
            case E_BUSY:
                printf("UART busy\n");
                break;
            case E_BAD_PARAM:
                printf("Bad parameter\n");
                break;
            case E_SHUTDOWN:
                printf("UART shutdown\n");
                break;
            default:
                printf("Unknown error\n");
                break;
        }
    }
    MXC_UART_ClearRXFIFO(GLOBAL_UART_REG);
}

void global_uart_init(void) {
    nextion_init();
    // Setup global UART request structure
    global_uart_req.uart = GLOBAL_UART_REG;
    global_uart_req.txData = global_tx_buffer;
    global_uart_req.rxData = global_rx_buffer;
    global_uart_req.txLen = BYTES;
    global_uart_req.rxLen = BYTES;
    global_uart_req.txCnt = 0;
    global_uart_req.rxCnt = 0;
    global_uart_req.callback = global_uart_callback;
    global_uart_interrupt_enable();
}

void global_uart_interrupt_disable(void) {
    NVIC_DisableIRQ(UART1_IRQn);
    MXC_UART_DisableInt(GLOBAL_UART_REG, RX_LVL);
}

// Process incoming UART data and route to appropriate handler
void handle_touch_event(uint8_t *rx_data) {
    if (!rx_data) return;
    page_t page = get_page(rx_data);
    uint8_t component = get_component(rx_data);
    uint8_t event = get_event(rx_data);
    // __disable_irq();
    printf("Processing: Event=0x%02X, Page=0x%02X, Component=0x%02X\n", 
        event, page, component);
    
    // Find the appropriate handler for this component
    for(int i = 0; i < sizeof(comp_table)/sizeof(screen_component); i++) {
		// printf("%d\n", page_id==comp_table[i].page);
		if(page==comp_table[i].page && component==comp_table[i].component) {
			comp_table[i].handler_function();
            // __enable_irq();
            current_game.is_busy = false;
			return;
		}
    }
}

void global_uart_main_loop(void) {
    printf("~~Main Loop~~\n\n");
    MXC_UART_TransactionAsync(&global_uart_req);
    while (1) {
        // Wait for UART interrupt
        while (!GLOBAL_UART_ISR_FLAG) {
        }
        printf("handling touch...");
        __disable_irq(); // debounce
        handle_touch_event(global_rx_buffer);
        // Reset flag and re-arm interrupt
        GLOBAL_UART_ISR_FLAG = 0;
        global_uart_req.txCnt = 0;
        global_uart_req.rxCnt = 0;
        MXC_UART_TransactionAsync(&global_uart_req);
        __enable_irq(); // must reenable interrupt

    }
}

uint8_t get_event(uint8_t *buff) {
    return buff[0];
}

page_t get_page(uint8_t *buff) {
    return buff[1];
}

uint8_t get_component(uint8_t *buff) {
    return buff[2];
}