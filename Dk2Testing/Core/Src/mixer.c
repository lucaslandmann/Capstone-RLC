///////////////////////////////////////////////////////////////////////////////
//
// @file mixer.c
// @brief This file contains the mixer implementation.
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
#include "mixer.h"

// T Y P E D E F S
typedef struct _trEffect
{
	bool  enable;
	float gain;
} trEffect;

typedef struct _trChannel
{
	// General.
	float volume;
	float pan;

	// Effects.
	trEffect rReverb;
	trEffect rDistortion;

	// Equalizer
	struct
	{
		bool  enable;
		float band[MIXER_EQ_BANDS];
	} rEqualizer;

} trChannel;

typedef struct _trMixer
{
	trChannel channel[MIXER_CHANNELS];
} trMixer;

// P R I V A T E  D A T A
static uint16_t adc1Vals[13] = {0};
static uint16_t adc4Vals[2]  = {0};
static uint8_t  pcmVals[8]   = {0};

// P R I V A T E  F U N C T I O N S
//
// @brief Get the mixer.
//
// @return pointer to mixer structure
//
static inline trMixer* mixer(void)
{
	static trMixer rMixer = {0};

    return &rMixer;
}

// P U B L I C   F U N C T I O N S
//
// @brief Intialize the mixer.
//
// @return enumerated error code
//
teError mixerInit(void)
{
	//HAL_SAI_Receive_DMA(bsp()->pSaiBlockB2,pcmVals,DIM(pcmVals));
	HAL_ADC_Start_DMA(bsp()->pAdc1, (uint32_t*)adc1Vals, DIM(adc1Vals));
	HAL_ADC_Start_DMA(bsp()->pAdc4, (uint32_t*)adc4Vals, DIM(adc4Vals));

	return eErrorNone;
}

//
// @brief Service the mixer.
//
// @return enumerated error code
//
bool mixerService(void)
{
	return true;
}
