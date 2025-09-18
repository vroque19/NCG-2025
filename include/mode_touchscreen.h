#ifndef _MODE_TOUCHSCREEN_H
#define _MODE_TOUCHSCREEN_H
#include "nextion.h"
#include "4131.h"

#define RING_0 "t12"
#define RING_1 "t13"
#define RING_2 "t14"
// // RING LOCATIONS FOR VISUALS
#define bottom_pos_y 162
#define mid_pos_y 139
#define top_pos_y 116
#define ring_0_pos_x 73
#define ring_1_pos_x 60
#define ring_2_pos_x 45
#define tower_distance 145 // distance between each tower
/*
usage: move <comp>,<x1>,<y1>,<x2>,<y2>,<priority>,<time>
move t14,45,162,45,100,5,1050
move t14,45,100,335,100,4,1050
move t14,335,100,335,162,3,1050
*/


/**
* @brief updates the main text box on the touchscreen page
* @return void
*/
void update_txt_box(char *val);
/**
* @brief clears all 3 buttons on touchscreen page after destination tower is selected
* @return void
*/
void clear_boxes(void);
void select_box(int tower_idx);
void nextion_move_rings(int source, int dest, int source_height, int dest_height, int ring_size);



#endif

