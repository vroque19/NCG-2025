#ifndef MOVES_H
#define MOVES_H
#include "nextion.h"
#include "stack.h"
#include "solenoid_driver.h"
#include "motors.h"
#include <stdint.h>

// TODO: change these to actual values
#define LIFTED_POS         1000000
#define RING_0_DEFAULT_POS 1211200
#define RING_1_DEFAULT_POS 1331200
#define RING_2_DEFAULT_POS 1454080
#define TOWER_0_POS 140880
#define TOWER_1_POS 307200
#define TOWER_2_POS 468480

// move ring from source tower to destination tower
void move_ring(uint8_t source_tower, uint8_t destination_tower);

void grab_ring(int ring_height);
// TODO
void drop_ring(void);
void move_to_tower(uint8_t tower_idx);

void move_to_home(void);
void wait_for_motor(tmc5272_dev_t device, tmc5272_motor_num_t motor, uint8_t tower_axis, int idx);

/**
 * TODO:
 * 1. Add moves (go to tower x, pick up ring, etc.) that call the TMC driver
 */
 #endif