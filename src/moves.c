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

#include "moves.h"
#include "TMC5272.h"
#include "game_logic.h"
#include "history.h"
#include "motors.h"
#include "mxc_delay.h"
#include "nextion.h"
#include "solenoid_driver.h"
#include <stdint.h>
#include <math.h>

// array of ring positions on y axis
uint32_t ring_pos_y[3] = {RING_2_DEFAULT_POS, RING_1_DEFAULT_POS, RING_0_DEFAULT_POS};
// array of tower positions on x axis
uint32_t tower_pos_x[3] = {TOWER_0_POS, TOWER_1_POS, TOWER_2_POS};


void move_ring(uint8_t source_tower, uint8_t destination_tower) {
    solenoid_on();
    printf("Moving ring physically from source tower {%d}.\n Top idx is {%d}\n\n", source_tower, current_game.towers[source_tower].top_idx);
    // move to source tower
    x_move(tower_pos_x[source_tower]);
    // move to the top ring
    MXC_DELAY_MSEC(50);
    y_move(ring_pos_y[current_game.towers[source_tower].top_idx]);

    // grab ring
    solenoid_off();
    // MXC_Delay((MXC_DELAY_MSEC(500)));
    // move to the top
    y_move(LIFTED_POS);
    // move to the destination tower
    uint32_t tower_pos = tower_pos_x[destination_tower];
    x_move(tower_pos);
    // move down
    // // drop ring
    y_move(RING_DROP_HEIGHT);
    solenoid_on();

    // move back up
    y_move(LIFTED_POS);

    printf("Successfully moved ring!!!\n\n\n");
    MXC_Delay(MXC_DELAY_MSEC(200));
    solenoid_off();
}

void move_to_home(void) {
    y_move(HOME_Y_POS);
    x_move(HOME_X_POS);
}

void y_move(uint32_t position) {
    tmc5272_rotateToPosition(tmc_devices.tmc_y, ALL_MOTORS, position, TMC_VEL_MAX, TMC_ACC_MAX);
    while(!tmc5272_isAtTargetPosition(tmc_devices.tmc_y, ALL_MOTORS)) {}
}
void x_move(uint32_t position) {
     tmc5272_rotateToPosition(tmc_devices.tmc_x, MOTOR_0, position, TMC_VEL_MAX, TMC_ACC_MAX);
    while(!tmc5272_isAtTargetPosition(tmc_devices.tmc_x, MOTOR_0)) {

    }
}

// TODO: make reset
void auto_reset_game(void) {
    int top = current_game.move_history.top_idx;
    printf("Resetting game\n%d, move:(%d,%d)\n", top, current_game.move_history.moves[top].source, current_game.move_history.moves[top].destination);
    while(current_game.move_history.top_idx >= 0) {
        move_tuple curr_move;
        pop_history(&current_game.move_history, &curr_move);
        move_ring(curr_move.destination, curr_move.source);
    }
}
