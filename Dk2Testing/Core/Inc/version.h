///////////////////////////////////////////////////////////////////////////////
//
// @file version.h
// @brief This file contains version definitions.
//
//  COPYRIGHT (c) 2022 Summit Scientific, Inc. All rights reserved.
//  NOTICE:  Contains confidential and proprietary information of
//           Summit Scientific, Inc. and is protected under the copyright
//           laws as an unpublished work.
//
// @author R. Van Rooyen
//
///////////////////////////////////////////////////////////////////////////////

#ifndef VERSION_H
#define VERSION_H

// M A C R O S
#define VERSION_CONF_OMEGA		0
#define VERSION_CONF_ALPHA		1
#define VERSION_CONF_BETA		2
#define VERSION_CONF_RELASE		3

#define VERSION_MAJOR			0
#define VERSION_MINOR			0
#define VERSION_BUILD			1
#define VERSION_CONF			VERSION_CONF_OMEGA

// P U B L I C  F U N C T I O N S
#ifdef __cplusplus
extern "C" {
#endif

const char * versionConfidence(uint16_t confidence);

#ifdef __cplusplus
}
#endif

#endif // VERSION_H
