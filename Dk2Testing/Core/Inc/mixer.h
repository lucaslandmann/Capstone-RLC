///////////////////////////////////////////////////////////////////////////////
//
// @file mixer.h
// @brief This file contains the mixer interface definition.
//
//  COPYRIGHT (c) 2022 Summit Scientific, Inc. All rights reserved.
//  NOTICE:  Contains confidential and proprietary information of
//           Summit Scientific, Inc. and is protected under the copyright
//           laws as an unpublished work.
//
// @author R. Van Rooyen
//
///////////////////////////////////////////////////////////////////////////////

#ifndef MIXER_H
#define MIXER_H

// D E F I N E S
#define MIXER_CHANNELS					(6)
#define MIXER_EQ_BANDS					(5)

// P U B L I C  F U N C T I O N S
#ifdef __cplusplus
extern "C" {
#endif

teError mixerInit(void);
bool    mixerService(void);

#ifdef __cplusplus
}
#endif

#endif // MIXER_H
