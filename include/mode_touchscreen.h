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
void nextion_change_ring_color(uint8_t ring_size, int color);




#endif

