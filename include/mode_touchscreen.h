#ifndef _MODE_TOUCHSCREEN_H
#define _MODE_TOUCHSCREEN_H
#include "nextion.h"
#include "4131.h"

#define TOWER_0_ID 0x07
#define TOWER_1_ID 0x08
#define TOWER_2_ID 0x09
#define TOGGLE_1TO2_ID 0x0A
#define PAGE1_ID 0x01
typedef struct {
    uint8_t page;
    uint8_t component;
    void (*handler_function)(void);
} screen_component;

void update_txt_box(char *val);

// Page 1 button functions
void handle_tower_0_btn(void);
void handle_tower_1_btn(void);
void handle_tower_2_btn(void);
void handle_toggle_1to2_btn(void);
void handle_touch_event(uint8_t page_id, uint8_t comp_id);
void clear_boxes(void);
// Page 2 button functions

int mode_touchscreen_run(void);

#endif

