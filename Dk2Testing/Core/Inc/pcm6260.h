///////////////////////////////////////////////////////////////////////////////
//
// @file pcm6260.h
// @brief This file contains the PCM6260 ADC interface definition.
//
//  COPYRIGHT (c) 2022 Summit Scientific, Inc. All rights reserved.
//  NOTICE:  Contains confidential and proprietary information of
//           Summit Scientific, Inc. and is protected under the copyright
//           laws as an unpublished work.
//
// @author R. Van Rooyen
//
///////////////////////////////////////////////////////////////////////////////

#ifndef PCM6260_H
#define PCM6260_H

// D E F I N E S
#define PCM6260_I2C_ADDRESS				(0x90)
#define PCM6260_RESET_WAIT				(1)

// P U B L I C  F U N C T I O N S
#ifdef __cplusplus
extern "C" {
#endif

teError pcm6260Init(void);

#ifdef __cplusplus
}
#endif

#endif // PCM6260_H
