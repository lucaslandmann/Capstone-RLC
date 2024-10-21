///////////////////////////////////////////////////////////////////////////////
//
// @file platform.h
// @brief This file contains the platform interface definition.
//
//  COPYRIGHT (c) 2022 Summit Scientific, Inc. All rights reserved.
//  NOTICE:  Contains confidential and proprietary information of
//           Summit Scientific, Inc. and is protected under the copyright
//           laws as an unpublished work.
//
// @author R. Van Rooyen
//
///////////////////////////////////////////////////////////////////////////////

#ifndef PLATFORM_H
#define PLATFORM_H

// D E F I N E S
#define PLATFORM_MFG                    "RLC"
#define PLATFORM_MFG_FULL               "RLC Audio Technologies"
#define PLATFORM_MDL					"0.1"
#define PLATFORM_DESCRIPTION			"STM32U5 Mixer Prototype"
#define PLATFORM_COPYRIGHT				"(c) 2024,"
#define PLATFORM_RIGHTS					"All Rights Reserved."
#define PLATFORM_TYPE                   "app"

#define PLATFORM_HEARTBEAT_INTERVAL		1000

// P U B L I C  F U N C T I O N S
#ifdef __cplusplus
extern "C" {
#endif

bool platformInit(trBsp *prBsp);
bool platformService(void);
void platformShutdown(void);

#ifdef __cplusplus
}
#endif

#endif // PLATFORM_H
