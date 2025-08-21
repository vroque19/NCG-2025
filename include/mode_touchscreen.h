#ifndef _MODE_TOUCHSCREEN_H
#define _MODE_TOUCHSCREEN_H
#include "nextion.h"
#include "4131.h"

#define TOWER_0_ID 0x07
#define TOWER_1_ID 0x08
#define TOWER_2_ID 0x09
#define TOGGLE_1TO2_ID 0x0A
#define PAGE1_ID 0x01

void update_txt_box(char *val);
// Page 1 button functions
void handle_tower_0_btn(void);
void handle_tower_1_btn(void);
void handle_tower_2_btn(void);
void clear_boxes(void);
// Page 2 button functions


#endif

