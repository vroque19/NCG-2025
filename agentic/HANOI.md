# Tower of Hanoi BFS Solver Implementation

This document describes the BFS-based optimal solver for Tower of Hanoi, data structures used, bugs found and fixed, and testing approach.

## Overview

The Tower of Hanoi solver uses **Breadth-First Search (BFS)** to find the optimal (shortest) solution from any arbitrary game state **back to the initial state** (all rings on tower 0). This is used to **UNDO/RESET** the game after a participant's attempt, allowing the system to automatically return to the starting configuration.

## Data Structures

### Queue (`queue.h`, `queue.c`)

**Purpose**: Implements FIFO queue for BFS algorithm.

**Structure**:
```c
typedef struct {
  int state[NUM_TOWERS][MAX_RINGS];  // Game state (3 towers × 3 rings)
  move_tuple moves[128];              // Move sequence to reach this state
  int moves_idx;                      // Number of moves in sequence
} Queue_Entry;

typedef struct {
  Queue_Entry entries[64];  // Fixed-size queue
  int front;                // Index to dequeue from
  int rear;                 // Index to enqueue at
} Queue;
```

**Key Functions**:
- `queue_init()` - Initialize empty queue
- `queue_push()` - Add entry to rear
- `queue_pop()` - Remove entry from front (returns pointer to entry)
- `queue_empty()` - Check if queue is empty
- `find_top_of_state(state[MAX_RINGS])` - Find highest non-zero index in tower (top ring)

**State Representation**: Each tower is an array where index 0 = bottom, index 2 = top. Ring values are weights (30, 60, 110 grams). Zero means empty position.

### Set (`set.h`, `set.c`)

**Purpose**: Track visited states to avoid cycles in BFS.

**Structure**:
```c
typedef struct {
  int idx;                                    // Number of states stored
  int items[256][NUM_TOWERS][MAX_RINGS];     // Array of states
} Set;
```

**Key Functions**:
- `set_init()` - Create empty set
- `set_add(set, state)` - Add state to set (no-op if already present)
- `in_set(set, state)` - Check if state exists in set
- `states_are_equal(state1, state2)` - Compare two states for equality

**Important Fix**: Changed `set_add()` at line 30 from `states_are_equal(&visited->items[i], state)` to `states_are_equal(visited->items[i], state)`. The `&` operator created wrong pointer type (`int (*)[3][3]` instead of `int (*)[3]`).

### History Stack (`history.h`, `history.c`)

**Purpose**: Store move sequence (used for solution output and move tracking).

**Structure**:
```c
typedef struct {
    int source;         // Starting tower (0-2)
    int destination;    // Destination tower (0-2)
} move_tuple;

typedef struct {
    move_tuple moves[MAX_MOVES];  // Stack of moves (max 20)
    int top_idx;                  // -1 means empty
} history_stack;
```

## BFS Algorithm (`optimal_solve()`)

### High-Level Flow

1. **Initialize**: Create queue and visited set
2. **Seed**: Add initial game state (from `current_game.towers`) to queue
3. **BFS Loop**:
   - Dequeue state
   - Check if goal reached → return solution
   - Generate all valid next moves from current state
   - For each unvisited next state:
     - Add to queue with updated move history
     - Mark as visited
4. **Termination**: Return when goal found (or queue empty)

### Goal State

All rings on tower 0 (initial state) in proper order (largest to smallest, bottom to top):
```c
int goal_state[NUM_TOWERS][MAX_RINGS] = {{0}};

// Dynamically built based on number of rings in game
// ring_weights = {30, 60, 110}
for (int i = 0; i < current_game.num_rings; i++) {
    goal_state[0][i] = (int)ring_weights[MAX_RINGS - 1 - i];
}

// For 3 rings: {110, 60, 30} on tower 0
// For 2 rings: {110, 60, 0} on tower 0
// For 1 ring:  {110, 0, 0} on tower 0
```

### Move Generation (`get_moves()`)

**Purpose**: Generate all legal moves from a given state.

**Algorithm**:
```
For each source tower (0-2):
  For each destination tower (0-2):
    Skip if source == destination
    Skip if source tower empty
    Find top ring on source (top_idx_src)
    Find top ring on destination (top_idx_dst)
    If destination empty OR source_ring < dest_ring:
      Create new state with move applied
      Store in output array
```

**Return**: Dynamically allocated array of `Queue_Entry` structures (max 6 possible moves).

**Memory**: Caller must `free()` the returned array.

## Bugs Found and Fixed

### 1. Type Mismatch in `set.c` (Line 30)

**Problem**: Passing `&visited->items[i]` created incompatible pointer type.
```c
// WRONG
if (states_are_equal(&visited->items[i], state))

// CORRECT
if (states_are_equal(visited->items[i], state))
```

**Root Cause**: `visited->items[i]` is `int[NUM_TOWERS][MAX_RINGS]` which decays to `int (*)[MAX_RINGS]`. Adding `&` creates `int (*)[NUM_TOWERS][MAX_RINGS]`.

**Compiler Warning**:
```
warning: passing argument 1 of 'states_are_equal' from incompatible pointer type
expected 'int (*)[3]' but argument is of type 'int (*)[3][3]'
```

### 2. Empty Queue Bug in Original `optimal_solve()`

**Problem**: Queue initialized empty but `while (!queue_empty(q))` entered immediately with no initial state.

**Fix**: Push initial game state to queue before BFS loop:
```c
Queue_Entry initial_entry;
// ... populate from current_game.towers ...
queue_push(q, initial_entry);
set_add(visited, initial_entry.state);
```

### 3. Goal Check Logic Error

**Problem**: Goal check was inside wrong part of loop and used incorrect state representation.

**Fix**: Check goal immediately after dequeue, use correct goal state format.

### 4. Memory Allocation in `get_moves()`

**Problem**: Original code allocated one `Queue_Entry` via `queue_entry_ctor()` but tried to fill multiple entries.

**Fix**: Allocate array of 6 entries (max possible moves: 3 towers × 2 destinations each):
```c
Queue_Entry *poss_entries = (Queue_Entry *)malloc(sizeof(Queue_Entry) * 6);
```

### 5. Loop Iteration Bug

**Problem**: Inner loop iterated over `MAX_RINGS` instead of `NUM_TOWERS`:
```c
for (int dst = 0; dst < MAX_RINGS; dst++)  // WRONG
```

**Fix**:
```c
for (int dst = 0; dst < NUM_TOWERS; dst++)  // CORRECT
```

### 6. Missing `stdlib.h` Include

**Problem**: `malloc()` and `free()` used without declaration.

**Fix**: Added `#include <stdlib.h>` to `game_logic.c`.

### 7. Missing Function Declaration

**Problem**: `get_moves()` not declared in header, causing implicit declaration warnings.

**Fix**: Added to `queue.h`:
```c
Queue_Entry *get_moves(int (*state)[MAX_RINGS], int *entry_idx);
```

### 8. Test State Setup Issue

**Problem**: Test cases manually setting up game states were calling `init_stack()` which resets `top_idx` and `ring_count` to 0, but does NOT zero out the `rings` array. Old ring values from `hanoi_init_game()` persisted, causing incorrect state comparisons.

**Symptom**: All test cases returned 0 moves because initial state appeared to match goal state (both had rings on tower 0 from previous initialization).

**Fix**: Added `memset()` to zero out rings array before manually setting up test states:
```c
for (int i = 0; i < NUM_TOWERS; i++) {
    init_stack(&current_game.towers[i]);
    memset(current_game.towers[i].rings, 0, sizeof(current_game.towers[i].rings));
}
```

### 9. Goal State Index Calculation Error

**Problem**: Initial goal state calculation used `ring_weights[MAX_RINGS - current_game.num_rings + i]` which gave wrong order.
- For 3 rings: `[0+i]` → `{30, 60, 110}` ❌ (should be `{110, 60, 30}`)

**Fix**: Changed to `ring_weights[MAX_RINGS - 1 - i]`:
- For 3 rings: `[2-i]` → `{110, 60, 30}` ✅
- For 1 ring: `[2]` → `{110, 0, 0}` ✅

## Testing Strategy

### Test File: `test_solver.c`

**Test Cases**:
1. **Helper Functions**:
   - `find_top_of_state()` - Find top ring index
   - `states_are_equal()` - State comparison
   - `get_moves()` - Move generation

2. **Solver for RESET/UNDO (back to tower 0)**:
   - TC1: Already at initial state → 0 moves ✅
   - TC2: Rings scattered across towers → finds path back ✅
   - TC3: One move away from reset (1 ring on tower 1) → 1 move ✅
   - TC4: All rings on tower 2 → 7 moves to reset ✅
   - TC5: Two rings already at start → 0 moves ✅
   - TC6: Single ring on tower 2 → 1 move to reset ✅

3. **Edge Cases**:
   - Different numbers of rings (1, 2, 3)
   - Partial states (rings on multiple towers)
   - Already at goal state

### Test Execution

Added to `test/run_test.sh`:
```bash
echo "\n5. Running Solver Tests..."
gcc -I ../include ../src/game_logic.c ../src/stack.c ../src/history.c \
    ../src/queue.c ../src/set.c test_api.c test_solver.c \
    -o solver_tests.out && ./solver_tests.out
```

### Current Test Results

✅ **ALL TESTS PASSING** (100% pass rate):
- Helper functions: 9/9 assertions passing
- Reset/undo solver: All scenarios working correctly
- Supports 1, 2, and 3 rings dynamically
- Correctly finds optimal path back to tower 0 from any valid state

## Known Limitations

1. **Goal is Tower 0 Only**: Solver only finds paths to initial state (tower 0). Does not support solving forward to tower 2.

2. **Fixed Queue Size**: Queue limited to 64 entries. For complex states, might overflow (though unlikely for 3 rings).

3. **Memory Leaks**: `get_moves()` allocates memory that caller must free. All callers properly free except on early returns.

4. **No Validation**: Assumes input state is valid (reachable). Invalid states may cause infinite loop or no solution.

## Future Improvements

1. **Dynamic Goal Generation**:
```c
int goal_state[NUM_TOWERS][MAX_RINGS] = {{0}};
for (int i = 0; i < current_game.num_rings; i++) {
    goal_state[2][i] = current_game.towers[0].rings[i]; // Copy ring weights
}
```

2. **Iterative Deepening**: For memory efficiency, use IDDFS instead of BFS.

3. **Bidirectional Search**: Search from both initial and goal states simultaneously.

4. **State Hashing**: Use hash table instead of linear search in Set for O(1) lookup.

## Integration with Physical System

The solver generates a **reset/undo** move sequence (`history_stack`) that can be executed by:

1. **After participant completes their attempt**: Call `optimal_solve()` to get sequence of moves to return to initial state
2. **Auto-reset execution**: For each move in solution:
   - Call motor control to move arm to source tower
   - Actuate solenoid to grip ring
   - Move to destination tower (always moving toward tower 0)
   - Release ring
   - Update game state via `hanoi_execute_move()`
3. **Verification**: Load cells verify physical state matches logical state after each move
4. **Ready for next participant**: System now reset to initial configuration (all rings on tower 0)

## File Locations

- **Implementation**: `src/game_logic.c` (lines 109-214)
- **Header**: `include/game_logic.h` (optimal_solve prototype)
- **Queue**: `src/queue.c`, `include/queue.h`
- **Set**: `src/set.c`, `include/set.h`
- **Tests**: `test/test_solver.c`
- **Test Runner**: `test/run_test.sh` (line 18-21)

## Building

```bash
# Build entire project (includes solver)
make

# Run tests only
cd test && ./run_test.sh

# Run solver tests specifically
cd test
gcc -I ../include ../src/game_logic.c ../src/stack.c ../src/history.c \
    ../src/queue.c ../src/set.c test_api.c test_solver.c -o solver_tests.out
./solver_tests.out
```

## Summary of Fixes

This implementation required fixing **9 major bugs**:

1. ✅ Type mismatch in `set.c` (pointer dereferencing)
2. ✅ Loop bounds error (iterating over wrong constant)
3. ✅ Memory allocation (single vs array of Queue_Entry)
4. ✅ Empty queue initialization
5. ✅ Goal check logic placement
6. ✅ Missing stdlib.h include
7. ✅ Missing function declarations
8. ✅ Test state setup (rings array not zeroed)
9. ✅ Goal state index calculation (wrong ring order)

**Result**: 100% test passing rate, optimal solver working correctly for reset/undo functionality.

## Key Insights

1. **BFS Guarantees Optimality**: Since all moves have equal cost, BFS finds shortest path.

2. **State Space Size**: For 3 rings, max states = C(6,3) × 3! ≈ 20 × 6 = 120 states (actually less due to ordering constraints). BFS explores efficiently.

3. **Move Validation**: Tower of Hanoi rule (no larger ring on smaller) enforced in `get_moves()` via comparison at line 127.

4. **Zero-Based Indexing**: Towers and rings use 0-based indexing. Tower 0 = start/goal (initial position), Tower 2 = rightmost.

5. **Purpose**: This solver is for **UNDO/RESET** functionality - finding optimal path back to initial state (all rings on tower 0) after a user attempt.
