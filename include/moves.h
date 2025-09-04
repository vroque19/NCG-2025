#ifndef MOVES_H
#define MOVES_H
#include "nextion.h"
#include "stack.h"
#include "history.h"
#include "game_logic.h"
#include "motors.h"
#include <stdint.h>

/**
 * @brief Handles movement from source tower to destination
 * @param source_tower the source tower (0-2)
 * @param destination_tower the destination tower (0-2)
 * @return void
 */
void hanoi_execute_move(uint8_t source_tower, uint8_t destination_tower);

void grab_ring(int ring_height);

void move_to_tower(uint8_t tower_idx);
/**
 * TODO:
 * 1. Add moves (go to tower x, pick up ring, etc.) that call the TMC driver
 */
 #endif