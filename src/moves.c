
#include "moves.h"
#include "TMC5272.h"
#include "game_logic.h"
#include "history.h"
#include "motors.h"
#include "mxc_delay.h"
#include "nextion.h"
#include "solenoid_driver.h"

// global array of ring positions on y axis
uint32_t ring_pos_y[3] = {RING_0_DEFAULT_POS, RING_1_DEFAULT_POS, RING_2_DEFAULT_POS};
// global array of tower positions on x axis
uint32_t tower_pos_x[3] = {TOWER_0_POS, TOWER_1_POS, TOWER_2_POS};


void move_ring(uint8_t source_tower, uint8_t destination_tower) {
    printf("Moving ring physically from source tower {%d}.\n Top idx is {%d}\n\n", source_tower, current_game.towers[source_tower].top_idx);
    // move to source tower
    tmc5272_rotateToPosition(tmc_devices.tmc_x, MOTOR_0, tower_pos_x[source_tower], TMC_VEL_MAX, TMC_ACC_MAX);
    while(!tmc5272_isAtTargetPosition(tmc_devices.tmc_x, MOTOR_0)) {}
    // move to the top ring
    tmc5272_rotateToPosition(tmc_devices.tmc_y, ALL_MOTORS, ring_pos_y[current_game.towers[source_tower].top_idx], TMC_VEL_MAX, TMC_ACC_MAX);
    while(!tmc5272_isAtTargetPosition(tmc_devices.tmc_y, ALL_MOTORS)) {}

    
    // grab ring
    solenoid_on();
    // MXC_Delay((MXC_DELAY_MSEC(500)));
    // move to the top
    tmc5272_rotateToPosition(tmc_devices.tmc_y, ALL_MOTORS, LIFTED_POS, TMC_VEL_MAX, TMC_ACC_MAX);
    while(!tmc5272_isAtTargetPosition(tmc_devices.tmc_y, ALL_MOTORS)) {}
    // move to the destination tower
    tmc5272_rotateToPosition(tmc_devices.tmc_x, MOTOR_0, tower_pos_x[destination_tower], TMC_VEL_MAX, TMC_ACC_MAX);
    while(!tmc5272_isAtTargetPosition(tmc_devices.tmc_x, MOTOR_0)) {

    }
    // drop ring
     tmc5272_rotateToPosition(tmc_devices.tmc_y, ALL_MOTORS, ring_pos_y[current_game.towers[destination_tower].top_idx], TMC_VEL_MAX, TMC_ACC_MAX);
    while(!tmc5272_isAtTargetPosition(tmc_devices.tmc_y, ALL_MOTORS)) {}
    solenoid_off();
    printf("Successfully moved ring!!!\n\n\n");
}

void move_to_home(void) {
    return;
}

// TODO: make reset