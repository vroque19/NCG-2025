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
#define LIFTED_POS         601200
#define RING_DROP_HEIGHT   696020
#define RING_0_DEFAULT_POS 1308160
#define RING_1_DEFAULT_POS 1492480
#define RING_2_DEFAULT_POS 1658880
#define TOWER_0_POS 122880
// 215040 
#define TOWER_1_POS 294400 
#define TOWER_2_POS 471040
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