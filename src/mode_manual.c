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

#include "mode_manual.h"
#include "TMC5272.h"
#include "game_logic.h"
#include "global_uart_handler.h"
#include "nextion.h"
#include "moves.h"
#include <stdbool.h>

// too slow
void update_tower_weights(void) {
    double *weights = poll_weights();
    for(int i = 0; i < 3; i++) {
        printf("Updated Tower %d Weight: %d\n", weights[i]);
        current_game.towers[i].total_weight = weights[i];
    }
}

// returns the source tower of the ring and updates tower weights
int get_tower_from_weight_delta(void) {
  double *weights = poll_weights();
  int tower = 10;
    for(int i = 0; i < 3; i++) {
        current_game.towers[i].total_weight = weights[i];
        // printf("Updated Tower %d Weight: %d\n", weights[i]);
        if (abs(current_game.towers[i].total_weight - weights[i]) >= 30) {
            tower = i;
        } 
    }
    return tower;
}




