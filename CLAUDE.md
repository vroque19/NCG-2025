# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

NCG-2025 is a **physical robotic Tower of Hanoi** system built on the MAX32650 microcontroller (Analog Devices MaximSDK). The system features:
- Physical rings, towers, and a robotic arm to move rings between towers
- Load cells under each tower to track ring placement by weight
- Nextion touchscreen display for game interaction
- Three game modes: touchscreen, manual (Tricoder), and automated

The project extends the TMC-API from Analog Devices' Trinamic (MIT licensed).

## Build System

This project uses the **Analog Devices MaximSDK build system** with Make.

### Environment Setup
- Set `MAXIM_PATH` environment variable to point to your MaximSDK installation
- Target microcontroller: MAX32650
- Default board: EvKit_V1

### Common Build Commands

```bash
# Build the project (creates NCG-2025.elf in build/ directory)
make

# Build optimized release version
make release

# Clean build artifacts
make clean

# Clean everything including libraries
make distclean

# Build with explicit paths if MAXIM_PATH is not set
MAXIM_PATH=/path/to/MaximSDK make
```

### Running Tests

All tests are located in the `test/` directory and run on the host machine (not on the microcontroller):

```bash
# Run all tests
cd test && ./run_test.sh

# Run individual test suites
cd test
gcc -I ../include ../src/stack.c test_api.c test_stack.c -o stack_tests.out && ./stack_tests.out
gcc -I ../include ../src/history.c test_api.c test_history.c -o history_tests.out && ./history_tests.out
gcc -I ../include ../src/game_logic.c ../src/stack.c ../src/queue.c ../src/set.c ../src/history.c test_api.c test_game_logic.c -o game_logic_tests.out && ./game_logic_tests.out
gcc -I ../include ../src/game_logic.c ../src/stack.c ../src/history.c ../src/queue.c ../src/set.c test_api.c test_queue.c -o queue_tests.out && ./queue_tests.out
```

## Code Architecture

### Hardware Abstraction & Drivers

**Motor Control (TMC5272)**
- `src/TMC5272.c`, `include/TMC5272.h` - TMC5272 stepper motor driver implementation
- `include/TMC5272_HW_Abstraction.h` - Hardware abstraction layer with extensive register definitions
- `src/motors.c`, `include/motors.h` - High-level motor control interface
- Three TMC5272 devices managed via SPI:
  - `tmc_x`: X-axis motor
  - `tmc_y`: Y-axis motor
  - `tmc_tc`: Tricoder motor (manual mode input)
- SPI communication on `MXC_SPI1` with separate chip select pins

**Load Cells (AD4131)**
- `src/4131.c`, `include/4131.h` - AD4131 ADC driver for load cell readings
- `src/load_cell.c`, `include/load_cell.h` - Load cell calibration and weight conversion
- Three load cells (one per tower) with calibration offsets and conversion factors
- Weight readings used to verify physical ring placement

**Display (Nextion)**
- `src/nextion.c`, `include/nextion.h` - Nextion touchscreen interface via UART
- `src/global_uart_handler.c`, `include/global_uart_handler.h` - UART communication handler
- Touch event handling with component IDs for buttons and text fields
- Display pages: main menu, touchscreen mode, manual mode, automated mode, scale calibration

**Solenoid**
- `src/solenoid_driver.c`, `include/solenoid_driver.h` - GPIO control for ring gripper solenoid

### Game Logic & Data Structures

**Core Game State (`src/game_logic.c`, `include/game_logic.h`)**
- `game_state_t` structure tracks complete game state:
  - Tower states (using tower_stack for each)
  - Move count, minimum moves (2^n - 1)
  - Game completion status
  - Move history
- Move validation logic ensures Tower of Hanoi rules (no larger ring on smaller)
- Physical/logical state reconciliation using load cell weights
- Optimal solver: `optimal_solve()` generates solution move sequence

**Tower Stack (`src/stack.c`, `include/stack.h`)**
- Fixed-size stack (MAX_RINGS = 3) for each tower
- Tracks rings by weight value, total tower weight
- Operations: `push_tower()`, `pop_tower()`, `peek_tower()`

**Move History (`src/history.c`, `include/history.h`)**
- Stack-based move history (MAX_MOVES = 20)
- Stores `move_tuple` with source/destination tower IDs
- Used for undo functionality and move tracking

**Queue & Set (`src/queue.c`, `include/queue.h`, `src/set.c`, `include/set.h`)**
- Queue: Used in optimal solver (BFS-based solution generation)
- Set: Tracks visited game states to avoid cycles

### Game Modes

**Mode Touchscreen (`src/mode_touchscreen.c`, `include/mode_touchscreen.h`)**
- User selects towers via touchscreen to make moves
- Validates and executes moves based on button presses

**Mode Manual (`src/mode_manual.c`, `include/mode_manual.h`)**
- Tricoder-based input (physical device for ring manipulation)
- Uses third TMC5272 motor for position tracking

**Mode Auto (`src/mode_auto.c`, `include/mode_auto.h`)**
- Demonstrates automated solving of Tower of Hanoi
- Executes optimal solution sequence with physical movements

**Handlers (`src/handlers.c`, `include/handlers.h`)**
- Event handlers for display interactions
- Routes touch events to appropriate mode handlers

**Moves (`src/moves.c`, `include/moves.h`)**
- Physical move execution (coordinates motors + solenoid)

### Entry Point

**main.c**
- Initialization sequence:
  1. SPI init for ADC/motors
  2. Load cell setup and ADC ID verification
  3. Motor initialization (commented out in current version)
  4. Global UART init for display
  5. Solenoid GPIO init
  6. Main event loop: `global_uart_main_loop()`
- Cleanup on exit (free load cell structures, shutdown SPI)

## Key Implementation Notes

### Weight-Based Ring Identification
Ring sizes are identified by weight (in grams):
- Size 1: ~30g
- Size 2: ~60g
- Size 3: ~110g

Load cells validate that physical state matches logical game state.

### Move Validation Flow
1. User/system initiates move (source → destination tower)
2. `hanoi_validate_move()` checks:
   - Source tower not empty
   - Destination different from source
   - Tower of Hanoi rule (no larger on smaller)
3. Physical execution via motors + solenoid
4. Load cell verification of ring placement
5. Update game state and move history

### Optimal Solving
The automated solver uses a BFS-based approach to find the optimal solution sequence. The solution is stored in a history_stack and executed move-by-move with physical robot movements.

## Testing Strategy

Tests are **host-based** (compiled with gcc for local execution, not cross-compiled for MAX32650). This allows rapid iteration on game logic without flashing the microcontroller.

Test coverage includes:
- Tower stack operations (push/pop/peek)
- Move history stack
- Game logic (move validation, win detection)
- Queue and set data structures

When adding new game logic, write host-based tests first in `test/` directory following the existing pattern.
- dont add too many comments, just enough to explain overly complicated code