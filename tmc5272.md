# NCG-2025 Hanoi Project

## TMC5272 Motor Control Driver

The TMC5272 driver included in this project provides a C API for a MAX32650FTHR to control multiple TMC5272 ICs, enabling: 

- IC initialization and application-specific configuration
- Motor movement via position & velocity control
- Trinamic Tricoder position feedback

The `TMC5272.h` file contains all functions that are necessary to use most of the TMC5272 features.

This driver establishes communications via the SPI1 bus. An example sketch is provided in `main.c`. 

Trinamic base API is provided by the [TMC-API repo](https://github.com/analogdevicesinc/TMC-API/blob/master/tmc/ic/TMC5272/README.md).

### Hardware Requirements

- MAX32650FTHR evaluation board
- (3) TMC5272-BOB (Break-Out Board)
- (5) Stepper motors

### Getting Started

#### 1. Hardware Setup

- Connect the TMC5272 to the MAX32650 FTHR board via SPI1 on SS 1, 2, 3.
- Wire stepper motors to the TMC5272 A/B & C/D outputs.
    - Ensure the A/b and C/D pairs are maintained
    - Wire the tricoder motors to match the polarity as its corresponding directional motor because polarity affects the direction in which the motor turns.

For the example project, use TMC5272 outputs SS1.AB, SS2.AB & SS2.CD, and SS3.AB & SS3.CD.

Provide 12V to the TMC5272 on VM. Provide 3.3V to the TMC5272 on VDD, via MAX32650FTHR.

> [!caution]
> Do not connect / disconnect a motor while power is enabled! This can cause current to flow into the TMC5272 drivers, potentially damaging the device.
>
> Additionally, turn on the 12V before providing 3.3V, and remove 3.3V before removing 12V. Otherwise, the board may draw power from the 3.3V rail. (I haven't had this ruin anything yet, but it certainly seems bad.)

#### 2. Build & Flash

This project uses a .gitignore in order to only track application-specific files. Copy into a blank project from the MaximSDK `Examples` folder. 

Refer to [MSDK User Guide](https://analogdevicesinc.github.io/msdk//USERGUIDE/#getting-started-with-visual-studio-code) for documentation on building and flashing an application to the MAX32650FTHR.

#### 3. Example Usage

The provided `main.c` demonstrates:

- Necessary setup steps:
    - Initializing SPI and TMC5272 devices using `tmc5272_dev_t`
    - Configuring motion profiles (velocity curves)
- Moving motors to absolute/relative positions
- Polling for arrival
- Basic Tricoder polling

See `TMC5272.h` for the full API.

### Application Notes

- Default register values are preset in `tmc5272_init()`. These can be left as-is, and should be good for most (NEMA17) stepper motors.

- Many API functions accept `ALL_MOTORS` to apply actions to both axes. Any functions that do not support this are noted in their function comments. Mostly these are `get()` functions for which it would not make sense to return a single value.

- Look at the `#define` statements in `main.c` and `TMC5272_SPI.h`; modify as needed.