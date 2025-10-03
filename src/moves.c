
#include "moves.h"
#include "TMC5272.h"
#include "game_logic.h"
#include "history.h"
#include "motors.h"
#include "mxc_delay.h"
#include "nextion.h"
#include "solenoid_driver.h"
#include <stdint.h>

// global array of ring positions on y axis
uint32_t ring_pos_y[3] = {RING_2_DEFAULT_POS, RING_1_DEFAULT_POS, RING_0_DEFAULT_POS};
// global array of tower positions on x axis
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
    // uint32_t pos_drop = ring_pos_y[current_game.towers[destination_tower].top_idx];
    // // drop ring
    // y_move(pos_drop);
    y_move(RING_DROP_HEIGHT);
    solenoid_on();

    // move back up
    printf("Successfully moved ring!!!\n\n\n");
    MXC_Delay(MXC_DELAY_MSEC(500));
    solenoid_off();
}

void move_to_home(void) {
    return;
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