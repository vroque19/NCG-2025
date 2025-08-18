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
#include "TMC5272_HW_Abstraction.h"

/* Functions */

// Low-Level Communications
// TMC-API
extern void tmc5272_readWriteSPI(uint16_t icID, uint8_t *tx_data, size_t dataLength, uint8_t* rx_data);
extern uint8_t tmc5272_getNodeAddress(uint16_t icID);

// TMC-API Wrappers
int32_t tmc5272_readRegister(uint16_t icID, uint8_t address, uint8_t* spi_status);
void tmc5272_writeRegister(uint16_t icID, uint8_t address, int32_t value, uint8_t* spi_status);

// Part Initialization
void tmc5272_init(uint16_t icID);
void tmc5272_configEmergencyStop(uint16_t icID, uint8_t motor, uint8_t isEnabled);

// Movement Commands
uint32_t tmc5272_getPosition(uint16_t icID, uint8_t motor);
int32_t tmc5272_getVelocity(uint16_t icID, uint8_t motor);
bool tmc5272_isAtTargetVelocity(uint16_t icID, uint8_t motor);
bool tmc5272_isAtTargetPosition(uint16_t icID, uint8_t motor);

void tmc5272_setHomePosition(uint16_t icID, uint8_t motor, uint32_t position);
void tmc5272_setVelocityCurve(uint16_t icID, uint8_t motor, uint32_t vmax, uint32_t amax);

// Velocity Mode
void tmc5272_rotateAtVelocity(uint16_t icID, uint8_t motor, int32_t velocity, uint32_t acceleration);

// Position Mode
void tmc5272_rotateToPosition(uint16_t icID, uint8_t motor, uint32_t position);
void tmc5272_rotateByMicrosteps(uint16_t icID, uint8_t motor, int32_t usteps);


/* Tricoder */
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

void tmc5272_tricoder_init(uint16_t icID, uint8_t motor, int32_t encoder_value);
void tmc5272_tricoder_setBEMFHysteresis(uint16_t icID, uint8_t motor, tmc5272_tricoder_bemf_hysteresis_t bemf_hysteresis);
void tmc5272_tricoder_setEncoderValue(uint16_t icID, uint8_t motor, int32_t encoder_value);
int32_t tmc5272_tricoder_getPosition(uint16_t icID, uint8_t motor);





// TMC-API Boilerplate: Field + Field Functions
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

static inline uint32_t tmc5272_fieldRead(uint16_t icID, RegisterField field)
{
	uint32_t value = tmc5272_readRegister(icID, field.address, NULL);

    return tmc5272_fieldExtract(value, field);
}

static inline uint32_t tmc5272_fieldUpdate(uint32_t data, RegisterField field, uint32_t value)
{
    return (data & (~field.mask)) | ((value << field.shift) & field.mask);
}

static inline void tmc5272_fieldWrite(uint16_t icID, RegisterField field, uint32_t value)
{
	uint32_t regValue = tmc5272_readRegister(icID, field.address, NULL);

	regValue = tmc5272_fieldUpdate(regValue, field, value);

    tmc5272_writeRegister(icID, field.address, regValue, NULL);
}

#endif /* TMC_IC_TMC5272_H_ */
