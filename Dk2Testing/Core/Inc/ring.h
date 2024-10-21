///////////////////////////////////////////////////////////////////////////////
//
// @file ring.h
// @brief This file contains the ring interface definition.
//
//  COPYRIGHT (c) 2022 Summit Scientific, Inc. All rights reserved.
//  NOTICE:  Contains confidential and proprietary information of
//           Summit Scientific, Inc. and is protected under the copyright
//           laws as an unpublished work.
//
// @author R. Van Rooyen
//
///////////////////////////////////////////////////////////////////////////////

#ifndef RING_H
#define RING_H

// T Y P E D E F S
typedef void *			thRing;

// P U B L I C  F U N C T I O N S
#ifdef __cplusplus
extern "C" {
#endif

thRing   ringInit(char *pBuffer, uint32_t length);
bool     ringEmpty(thRing hRing);
uint32_t ringSpace(thRing hRing);
bool     ringFull(thRing hRing);
uint32_t ringWrite(thRing hRing, const char *pBuffer, const uint32_t length);
uint32_t ringRead(thRing hRing, char *pBuffer, const uint32_t length);

#ifdef __cplusplus
}
#endif

#endif // RING_H
