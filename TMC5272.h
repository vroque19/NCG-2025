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
#include <stdbool.h>
#include <stddef.h>
#include "TMC5272_SPI.h"
#include "TMC5272_HW_Abstraction.h"

/**** Macros ****/

// Establishes a loop to apply the same logic to every motor available on a device.
// Usage:
//     FOR_EACH_MOTOR(m) { motor_function(device, m) }  
// where m goes in place of MOTOR_0, MOTOR_1, etc.
#define FOR_EACH_MOTOR(motor_var) \
    for (uint8_t motor_var = 0; motor_var < TMC5272_MOTORS; motor_var++)


/**** Enums & Typedefs ****/

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

/* TMC-API Wrappers */
// ALL_MOTORS unsupported.
int32_t tmc5272_readRegister(tmc5272_dev_t* tmc5272_dev, uint8_t address, uint8_t* spi_status);
// ALL_MOTORS unsupported.
void tmc5272_writeRegister(tmc5272_dev_t* tmc5272_dev, uint8_t address, int32_t value, uint8_t* spi_status);

/* Part Initialization */

void tmc5272_init(tmc5272_dev_t* tmc5272_dev);
void tmc5272_setMotorDirection(tmc5272_dev_t* tmc5272_dev, tmc5272_motor_num_t motor, tmc5272_motor_dir_t dir);
void tmc5272_configEmergencyStop(tmc5272_dev_t* tmc5272_dev, tmc5272_motor_num_t motor, uint8_t isEnabled);


/* Movement Commands */
// ALL_MOTORS unsupported.
uint32_t tmc5272_getPosition(tmc5272_dev_t* tmc5272_dev, tmc5272_motor_num_t motor);
// ALL_MOTORS unsupported.
int32_t tmc5272_getVelocity(tmc5272_dev_t* tmc5272_dev, tmc5272_motor_num_t motor);

// Warning: Velocity_reached returns true if a stall is detected by SG2. 
bool tmc5272_isAtTargetVelocity(tmc5272_dev_t* tmc5272_dev, tmc5272_motor_num_t motor);
bool tmc5272_isAtTargetPosition(tmc5272_dev_t* tmc5272_dev, tmc5272_motor_num_t motor);
void tmc5272_setPositionToValue(tmc5272_dev_t* tmc5272_dev, tmc5272_motor_num_t motor, uint32_t value);
void tmc5272_setHomePosition(tmc5272_dev_t* tmc5272_dev, tmc5272_motor_num_t motor);
void tmc5272_setVelocityCurve(tmc5272_dev_t* tmc5272_dev, tmc5272_motor_num_t motor, uint32_t vmax, uint32_t amax);

/* Velocity Mode */

void tmc5272_rotateAtVelocity(tmc5272_dev_t* tmc5272_dev, tmc5272_motor_num_t motor, int32_t velocity, uint32_t acceleration);

/* Position Mode */

void tmc5272_rotateToPosition(tmc5272_dev_t* tmc5272_dev, tmc5272_motor_num_t motor, uint32_t position);
void tmc5272_rotateByMicrosteps(tmc5272_dev_t* tmc5272_dev, tmc5272_motor_num_t motor, int32_t usteps);


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
void tmc5272_configureStallGuard2(tmc5272_dev_t* tmc5272_dev, tmc5272_motor_num_t motor, int8_t SGT, uint32_t TCOOLTHRS, bool isFiltered);
void tmc5272_setStallGuard2(tmc5272_dev_t* tmc5272_dev, tmc5272_motor_num_t motor, bool isEnabled);
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
	uint32_t value = tmc5272_readRegister(tmc5272_dev, field.address, NULL);

    return tmc5272_fieldExtract(value, field);
}

static inline uint32_t tmc5272_fieldUpdate(uint32_t data, RegisterField field, uint32_t value)
{
    return (data & (~field.mask)) | ((value << field.shift) & field.mask);
}

static inline void tmc5272_fieldWrite(tmc5272_dev_t* tmc5272_dev, RegisterField field, uint32_t value)
{
	uint32_t regValue = tmc5272_readRegister(tmc5272_dev, field.address, NULL);

	regValue = tmc5272_fieldUpdate(regValue, field, value);

    tmc5272_writeRegister(tmc5272_dev, field.address, regValue, NULL);
}

#endif /* TMC_IC_TMC5272_H_ */
