#ifndef MOVES_H
#define MOVES_H
#include "nextion.h"
#include "stack.h"
#include "solenoid_driver.h"
#include "motors.h"
#include <stdint.h>

// TODO: change these to actual values
#define LIFTED_POS         478960
#define RING_DROP_HEIGHT 581120
#define RING_0_DEFAULT_POS 1208320
#define RING_1_DEFAULT_POS 1369600
#define RING_2_DEFAULT_POS 1543680
#define TOWER_0_POS 143360
#define TOWER_1_POS 307200
#define TOWER_2_POS 460800

// move ring from source tower to destination tower
void move_ring(uint8_t source_tower, uint8_t destination_tower);

void grab_ring(int ring_height);
// TODO
void drop_ring(void);
void move_to_tower(uint8_t tower_idx);

void move_to_home(void);

void x_move(uint32_t position);
void y_move(uint32_t position);


/**
 * TODO:
 * 1. Add moves (go to tower x, pick up ring, etc.) that call the TMC driver
 */
 #endif