#include "mode_auto.h"
#include "game_logic.h"
#include <stdio.h>
#include "stack.h"

void print_move(int source, int destination) {
    printf("%d->%d", source, destination);
}
void auto_solve_hanoi(int num_rings, int source, int destination) {
    if(num_rings == 1) {
        print_move(source, destination);
    } else {
        int aux = MAX_RINGS - (source + destination);
        auto_solve_hanoi(num_rings - 1, source, aux);
        print_move(source, destination);
        auto_solve_hanoi(num_rings - 1, aux, destination);
    }
}