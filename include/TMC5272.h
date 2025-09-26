/*******************************************************************************
* Copyright © 2019 TRINAMIC Motion Control GmbH & Co. KG
* (now owned by Analog Devices Inc.),
*
* Copyright © 2024 Analog Devices Inc. All Rights Reserved.
* This software is proprietary to Analog Devices, Inc. and its licensors.
*******************************************************************************/

#ifndef TMC_IC_TMC5272_H_
#define TMC_IC_TMC5272_H_

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>

#include "spi.h"
#include "mxc_device.h"
#include "mxc_pins.h"

#include "TMC5272_HW_Abstraction.h"

/**** Definitions ****/

// SPI Settings
#define SPI_SPEED           10000   // Bit Rate (Max.: 1,850,000)
#define TMC5272_SPI_SIZE    5       // Bytes per transaction
#define SPI_NUM_CLIENTS     3       // Max SS line being used 


/**** Macros ****/

// Establishes a loop to apply the same logic to every motor available on a device.
// Usage:
//     FOR_EACH_MOTOR(m) { motor_function(m) }  
// where m goes in place of MOTOR_0, MOTOR_1, etc.
#define FOR_EACH_MOTOR(motor_var) \
    for (uint8_t motor_var = 0; motor_var < TMC5272_MOTORS; motor_var++)


/**** Data Structures ****/

typedef enum {
    POSITION_MODE,
    VELOCITY_POS_MODE,
    VELOCITY_NEG_MODE,
    HOLD_MODE
} tmc5272_rampmode_t;

/* Shadow registers locally hold TMC5272 device state.
 * Note: Arrays describe register settings per motor. 
 */
typedef struct {
    tmc5272_rampmode_t rampmode[TMC5272_MOTORS];
    bool axis_interpolation;
    bool ramp_syn_pos_update;
    
    uint32_t vmax[TMC5272_MOTORS];
    uint32_t amax[TMC5272_MOTORS];
    
    uint32_t xtarget[TMC5272_MOTORS];
} tmc5272_shadow_t;

typedef struct  {
    mxc_spi_regs_t* spi_port;           // SPI Port
    mxc_gpio_cfg_t* gpio_cfg_spi;       // GPIO Config for SPI port (incl. all SS lines)
    uint8_t ss_index;                   // SPI SS pin index
    tmc5272_shadow_t shadow;
} tmc5272_dev_t;

typedef enum {
    MOTOR_0,
    MOTOR_1,
    ALL_MOTORS  // Warning: Check function commentary -- not supported by certain functions.
} tmc5272_motor_num_t;

typedef enum {
    MOTOR_DIR_NORMAL,
    MOTOR_DIR_INVERT
} tmc5272_motor_dir_t;

typedef enum {
    BEMF_HYST_25mV,	
	BEMF_HYST_10mV,	
	BEMF_HYST_50mV,	
	BEMF_HYST_75mV,	
	BEMF_HYST_100mV,
	BEMF_HYST_150mV,
	BEMF_HYST_200mV,
	BEMF_HYST_250mV

} tmc5272_tricoder_bemf_hysteresis_t;


/**** Functions *****/

/* Low-Level Communications */
void tmc5272_SPI_init(tmc5272_dev_t* tmc5272_dev);
void tmc5272_SPI_readWrite(tmc5272_dev_t* tmc5272_dev, uint8_t* tx_data, uint8_t dataLength, uint8_t* rx_data);

/* TMC-API Wrappers */
// ALL_MOTORS unsupported.
int32_t tmc5272_readRegister(tmc5272_dev_t* tmc5272_dev, uint8_t address);
// ALL_MOTORS unsupported.
uint8_t tmc5272_writeRegister(tmc5272_dev_t* tmc5272_dev, uint8_t address, int32_t value);

void tmc5272_dumpRegisters(tmc5272_dev_t* tmc5272_dev);

/* Part Initialization */

void tmc5272_init(tmc5272_dev_t* tmc5272_dev);
void tmc5272_setMotorPolarity(tmc5272_dev_t* tmc5272_dev, tmc5272_motor_num_t motor, tmc5272_motor_dir_t dir);
void tmc5272_setEmergencyStop(tmc5272_dev_t* tmc5272_dev, tmc5272_motor_num_t motor, uint8_t isEnabled);

// Note: Motor must enter positioning mode first before enabling synchronization.
// Recommendation is to do this by using rotateByMicrosteps() to move by 0, 
// and then enable synchronous mode.
//
// Note: Synchronous movement is automatically implemented in velocity mode when
// using ALL_MOTORS.
//
// Warning: 
// - Synchronous positioning will be disabled if velocity mode is used.
// - Synchronous mode uses a shadow register onboard the TMC5272, which is not updated
//   until M1's target position is written. Therefore, if you do not write the target
//   position of M0 **before** enabling synchronized positioning, the IC will use the
//   last-written target position, causing the motors to have unintended
//   movement on their first positional move command.
void tmc5272_setSynchronizedPositioning(tmc5272_dev_t* tmc5272_dev, bool isEnabled);

/* Movement Commands */
// ALL_MOTORS unsupported.
uint32_t tmc5272_getPosition(tmc5272_dev_t* tmc5272_dev, tmc5272_motor_num_t motor);
// ALL_MOTORS unsupported.
int32_t tmc5272_getVelocity(tmc5272_dev_t* tmc5272_dev, tmc5272_motor_num_t motor);

// Warning: Velocity_reached returns true if a stall is detected by SG2. 
bool tmc5272_isAtTargetVelocity(tmc5272_dev_t* tmc5272_dev, tmc5272_motor_num_t motor);
bool tmc5272_isAtTargetPosition(tmc5272_dev_t* tmc5272_dev, tmc5272_motor_num_t motor);
void tmc5272_setPositionValue(tmc5272_dev_t* tmc5272_dev, tmc5272_motor_num_t motor, uint32_t value);

// Only use to modify a velocity curve.
void tmc5272_setVelocityCurve(tmc5272_dev_t* tmc5272_dev, tmc5272_motor_num_t motor, uint32_t vmax, uint32_t amax);

/* Velocity Mode */

// Note: This function clears syn_pos_update and axis_interpolation bitfields. (Not supported by the IC.)
void tmc5272_rotateAtVelocity(tmc5272_dev_t* tmc5272_dev, tmc5272_motor_num_t motor, int32_t velocity, uint32_t acceleration);

/* Position Mode */

void tmc5272_rotateToPosition(tmc5272_dev_t* tmc5272_dev, tmc5272_motor_num_t motor, uint32_t position, uint32_t velocity, uint32_t acceleration);
void tmc5272_rotateByMicrosteps(tmc5272_dev_t* tmc5272_dev, tmc5272_motor_num_t motor, int32_t usteps, uint32_t velocity, uint32_t acceleration);


/* StallGuard */

// SGT = [-64, 63]
// SGT controls when the IC considers a "stall" has occured. Higher = less sensitive; lower = more sensitive (stalls are detected sooner).
//
// TCOOLTHRS = 20 bits [0, 65535]
// TCOOLTHRS disables stall detection until the TSTEP (motor speed in ticks) is faster (less) than TCOOLTHRS.
// TSTEP is inversely prop to rotation velocity, so lower values of TCOOLTHRS require higher speeds to enable SG2.
//
// Sample TSTEP values for 200 steps/rev stepper, 12.5 MHz clock: 
// - TSTEP = 43.7 <--> 300k velocity (335.27 rpm)
// - TSTEP = 65.5 <--> 200k velocity (223.52 rpm)
// - TSTEP = 131.1 <--> 100k velocity (111.76 rpm)
void tmc5272_sg_configureStallGuard2(tmc5272_dev_t* tmc5272_dev, tmc5272_motor_num_t motor, int8_t SGT, uint32_t TCOOLTHRS, bool isFiltered);
void tmc5272_sg_setStallGuard2(tmc5272_dev_t* tmc5272_dev, tmc5272_motor_num_t motor, bool isEnabled);

// ALL_MOTORS unsupported.
uint16_t tmc5272_sg_getSGValue(tmc5272_dev_t* tmc5272_dev, tmc5272_motor_num_t motor);
bool tmc5272_sg_isStalled(tmc5272_dev_t* tmc5272_dev, tmc5272_motor_num_t motor);
void tmc5272_sg_clearStall(tmc5272_dev_t* tmc5272_dev, tmc5272_motor_num_t motor);

/* Tricoder */

void tmc5272_tricoder_init(tmc5272_dev_t* tmc5272_dev, tmc5272_motor_num_t motor);
uint8_t tmc5272_tricoder_isCoilShortVS(tmc5272_dev_t* tmc5272_dev, tmc5272_motor_num_t motor);
void tmc5272_tricoder_resetFromCoilShort(tmc5272_dev_t* tmc5272_dev, tmc5272_motor_num_t motor);
void tmc5272_tricoder_setBEMFHysteresis(tmc5272_dev_t* tmc5272_dev, tmc5272_motor_num_t motor, tmc5272_tricoder_bemf_hysteresis_t bemf_hysteresis);
void tmc5272_tricoder_setEncoderValue(tmc5272_dev_t* tmc5272_dev, tmc5272_motor_num_t motor, int32_t encoder_value);

// ALL_MOTORS unsupported.
int32_t tmc5272_tricoder_getPosition(tmc5272_dev_t* tmc5272_dev, tmc5272_motor_num_t motor);





/* TMC-API Boilerplate: Field + Field Functions */
typedef struct
{
    uint32_t mask;
    uint8_t shift;
    uint8_t address;
    bool isSigned;
} RegisterField;

static inline uint32_t tmc5272_fieldExtract(uint32_t data, RegisterField field)
{
    uint32_t value = (data & field.mask) >> field.shift;

    if (field.isSigned)
    {
        // Apply signedness conversion
        uint32_t baseMask = field.mask >> field.shift;
        uint32_t signMask = baseMask & (~baseMask >> 1);
        value = (value ^ signMask) - signMask;
    }

    return value;
}

static inline uint32_t tmc5272_fieldRead(tmc5272_dev_t* tmc5272_dev, RegisterField field)
{
	uint32_t value = tmc5272_readRegister(tmc5272_dev, field.address);

    return tmc5272_fieldExtract(value, field);
}

static inline uint32_t tmc5272_fieldUpdate(uint32_t data, RegisterField field, uint32_t value)
{
    return (data & (~field.mask)) | ((value << field.shift) & field.mask);
}

static inline void tmc5272_fieldWrite(tmc5272_dev_t* tmc5272_dev, RegisterField field, uint32_t value)
{
	uint32_t regValue = tmc5272_readRegister(tmc5272_dev, field.address);

	regValue = tmc5272_fieldUpdate(regValue, field, value);

    tmc5272_writeRegister(tmc5272_dev, field.address, regValue);
}

#endif /* TMC_IC_TMC5272_H_ */
