///////////////////////////////////////////////////////////////////////////////
//
// @file macros.h
// @brief This file contains macro declarations and definitions.
//
//  COPYRIGHT (c) 2022 Summit Scientific, Inc. All rights reserved.
//  NOTICE:  Contains confidential and proprietary information of
//           Summit Scientific, Inc. and is protected under the copyright
//           laws as an unpublished work.
//
// @author R. Van Rooyen
//
///////////////////////////////////////////////////////////////////////////////

#ifndef MACROS_H
#define MACROS_H

// M A C R O S
#define DIM(a)					(sizeof(a)/sizeof(*a))

#define BYTE_SWAP(v)			(((v & 0xff) << 8) | ((v & 0xff00) >> 8))
#define LOW_BYTE(v)				(uint8_t)((v) & 0xff)
#define HIGH_DWORD(v)			(uint32_t)(((v) & 0xffffffff00000000) >> 32)
#define LOW_DWORD(v)			(uint32_t)((v) & 0x00000000ffffffff)
#define ADDRESS_DWORD(v)		(uint32_t)((v) >> 8)
#define DATA_DWORD(v)			(uint8_t)((v) & 0xff)

#define BITS_PER_BYTE           8

#define BIGLITTLESWAP16(a) 		(((uint16_t)(a) & 0xff00) >> 8) | \
								(((uint16_t)(a) & 0x00ff) << 8)
#define BIGLITTLESWAP32(a) 		(((uint32_t)(a) & 0xff000000) >> 24) | \
								(((uint32_t)(a) & 0x00ff0000) >> 8)  | \
								(((uint32_t)(a) & 0x0000ff00) << 8)  | \
								(((uint32_t)(a) & 0x000000ff) << 24)

#define HTONL(a) 				BIGLITTLESWAP32(a)
#define NTOHL(a) 				HTONL(a)
#define HTONS(a) 				BIGLITTLESWAP16(a)
#define NTOHS(a) 				HTONS(a)

#endif // MACROS_H
