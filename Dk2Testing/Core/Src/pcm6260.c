///////////////////////////////////////////////////////////////////////////////
//
// @file pcm6260.c
// @brief This file contains the PCM6260 ADC implementation.
//
//  COPYRIGHT (c) Summit Scientific, Inc. All rights reserved.
//  NOTICE:  Contains confidential and proprietary information of
//           Summit Scientific, Inc. and is protected under the copyright
//           laws as an unpublished work.
//
// @author R. Van Rooyen
//
///////////////////////////////////////////////////////////////////////////////

// P R O J E C T  I N C L U D E S
#include "common.h"
#include "main.h"
#include "pcm6260.h"

// T Y P E D E F S
typedef struct _trPcm6260Config
{
	uint8_t address;
	uint8_t data;
} trPcm6260Config;

// P R I V A T E  D A T A
static trPcm6260Config grPcm6260Config[] =
{
		{0x00, 0x00},
		{0x02, 0x81},
		{0x28,0x10},
		{0x3C,0x18},
		{0x41,0x18},
		{0x46,0x18},
		{0x4B, 0x18},
		{0x73,0xF0},
		{0x74, 0xF0},
		{0x75, 0xE0}
		/*
		// Select Page 0
	    { 0x00, 0x00 },
	    { 0x02, 0x81 },
	// 2s Delay After Disabling Sleep
	    { 0x07, 0x60 },
	// ASI Output CH5
	    { 0x0f, 0x20 },
	// ASI Output CH6
	    { 0x10, 0x21 },
	// ASI Configuration
	    { 0x13, 0x07 },
	    { 0x14, 0x38 },
	    { 0x17, 0x20 },
	    { 0x1a, 0x04 },
	    { 0x1b, 0x0c },
	    { 0x1c, 0xc0 },
	    { 0x1e, 0x82 },
	    { 0x1f, 0xb0 },
	// Micbias Configuration
	    { 0x3b, 0x70 },
	// CH1 CFG, Gain, Volume, Gain cal, phase cal
	    { 0x3c, 0xa0 },
	// CH2 CFG, Gain, Volume, Gain cal, phase cal
	    { 0x41, 0xa0 },
	// CH3 CFG, Gain, Volume, Gain cal, phase cal
	    { 0x46, 0xa0 },
	// CH4 CFG, Gain, Volume, Gain cal, phase cal
	    { 0x4b, 0xa0 },
	// CH5 CFG, Gain, Volume, Gain cal, phase cal
	    { 0x50, 0xb0 },
	// CH6 CFG, Gain, Volume, Gain cal, phase cal
	    { 0x55, 0xb1 },
	// Enable Diagnostics
	    { 0x64, 0xfc },
	// Input Channel Enable
	    { 0x73, 0xf0 },
	    { 0x74, 0xf0 },
	// Power up/down
	// Select page 0
	    { 0x00, 0x00 },
	    { 0x75, 0xa0 },
		*/
};

// P U B L I C   F U N C T I O N S
//
// @brief Intialize the ADC.
//
// @return enumerated error code
//
teError pcm6260Init(void)
{
	bspMillisecondDelay(100);
	HAL_GPIO_WritePin(ADC_Power_On_GPIO_Port, ADC_Power_On_Pin, GPIO_PIN_SET);
	bspMillisecondDelay(100);
	int index;

	// Wait for a bit and take the ADC out of reset.

	// Iterate over configuration.
	for (index = 0; index < DIM(grPcm6260Config); index++)
	{
		// Set register value.
		if (!bspI2cWrite(PCM6260_I2C_ADDRESS,
			(uint8_t*)&grPcm6260Config[index],
			sizeof(grPcm6260Config[index])))
		{
			return eErrorInit;
		}
		bspMillisecondDelay(100);
	}

	return eErrorNone;
}
