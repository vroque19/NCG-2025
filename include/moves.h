#ifndef MOVES_H
#define MOVES_H
#include "nextion.h"
#include "stack.h"
#include "solenoid_driver.h"
#include "motors.h"
#include <stdint.h>

// TODO: change these to actual values
#define RING_0_DEFAULT_POS 10
#define RING_1_DEFAULT_POS 20
#define RING_2_DEFAULT_POS 300
#define TOWER_0_POS -133120
#define TOWER_1_POS -279040
#define TOWER_2_POS -432640

// move ring from source tower to destination tower
void move_ring(uint8_t source_tower, uint8_t destination_tower);

void grab_ring(int ring_height);
// TODO
void drop_ring(void);
void move_to_tower(uint8_t tower_idx);

void move_to_home(void);

/**
 * TODO:
 * 1. Add moves (go to tower x, pick up ring, etc.) that call the TMC driver
 */
 #endif