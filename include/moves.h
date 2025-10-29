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

#ifndef MOVES_H
#define MOVES_H
#include "handlers.h"
#include "nextion.h"
#include "stack.h"
#include "solenoid_driver.h"
#include "motors.h"
#include <stdint.h>

// TODO: change these to actual values
// #define LIFTED_POS         0
// #define RING_DROP_HEIGHT   50000
// #define RING_0_DEFAULT_POS 100000
// #define RING_1_DEFAULT_POS 330000
// #define RING_2_DEFAULT_POS 650240
#define X_MIN_POS          10240
#define X_MAX_POS          596480
#define Y_MIN_POS          300000
#define Y_MAX_POS          1650700

#define HOME_X_POS         100000
#define HOME_Y_POS         400000
#define LIFTED_POS         600000
#define RING_DROP_HEIGHT   686020
#define RING_0_DEFAULT_POS 1346220
#define RING_1_DEFAULT_POS 1529300
#define RING_2_DEFAULT_POS 1649600
#define TOWER_0_POS 122980
// 215040 
#define TOWER_1_POS 296160  
#define TOWER_2_POS 478000
#define MANUAL_MODE_ASSISTANCE_THRESH 50000

// move ring from source tower to destination tower
void move_ring(uint8_t source_tower, uint8_t destination_tower);

void grab_ring(int ring_height);
// TODO
void drop_ring(void);
void move_to_tower(uint8_t tower_idx);

void move_to_home(void);

void x_move(uint32_t position);
void y_move(uint32_t position);

// automated ring reset
void auto_reset_game(void);

// get source tower
int get_source_tower(int pos_x);
 #endif
