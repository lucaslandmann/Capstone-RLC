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
	{0x02, 0x01},	// Set to Awake
	{0x28, 0x00},	// Power down mic bias and ADC channels on fault
	{0x07, 0x60},	// Set to i2s signaling mode
	{0x3B, 0x70},	// Set Micbias = 5V
	{0x3C, 0xBC},	// Config channel 1
	{0x41, 0xBC},	// Config channel 2
	{0x46, 0xBC},	// Config channel 3
	{0x4B, 0xBC},	// Config channel 4
	{0x50, 0xBC},	// Config channel 5
	{0x55, 0xBC},	// Config channel 6
	{0x73, 0xFC},	// Enable input channel 1 to 2 I2C
	{0x74, 0xFC},	// Enable output channel 1 to 2 ASI
	{0x75, 0xE0},	// Enable power
	{0x64, 0xFE}
	/*
	{0x02, 0x01},
		{0x73, 0xFC},
		{0x74, 0xFC},
		{0x75, 0x60}
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
