///////////////////////////////////////////////////////////////////////////////
//
// @file error.h
// @brief This file contains error definitions.
//
//  COPYRIGHT (c) 2022 Summit Scientific, Inc. All rights reserved.
//  NOTICE:  Contains confidential and proprietary information of
//           Summit Scientific, Inc. and is protected under the copyright
//           laws as an unpublished work.
//
// @author R. Van Rooyen
//
///////////////////////////////////////////////////////////////////////////////

#ifndef ERROR_H
#define ERROR_H

// E N U M S
//
// NOTE: Enumerated codes must be in sync with rError table in error.c.
//
typedef enum
{
    eErrorNone           = 0,
    eErrorAbort          = 1,
	eErrorAddress        = 2,
	eErrorArgument       = 3,
	eErrorAuthenticate   = 4,
	eErrorChannel        = 5,
	eErrorCrc            = 6,
    eErrorDone           = 7,
	eErrorEmpty          = 8,
	eErrorErase          = 9,
	eErrorFault          = 10,
    eErrorFull           = 11,
    eErrorHash           = 12,
	eErrorInit           = 13,
	eErrorInvalid        = 14,
	eErrorLength         = 15,
	eErrorMagic          = 16,
	eErrorNotFound       = 17,
	eErrorOutOfResources = 18,
	eErrorParameter      = 19,
	eErrorParse          = 20,
	eErrorProgram        = 21,
	eErrorReadoutLevel   = 22,
	eErrorRead           = 23,
	eErrorReceive        = 24,
	eErrorSend           = 25,
	eErrorSelfTest       = 26,
	eErrorStart          = 27,
    eErrorState          = 28,
	eErrorStop           = 29,
    eErrorTemperature    = 30,
	eErrorTimeout        = 31,
	eErrorType           = 32,
	eErrorUnsupported    = 33,
    eErrorUnknown        = 34,
    eErrorVerify         = 35,
    eErrorVersion        = 36,
    eErrorVoltage        = 37,
    eErrorWrite          = 38,
    eErrorWriteProtect   = 39
} teError;

// P U B L I C  F U N C T I O N S
#ifdef __cplusplus
extern "C" {
#endif

const char * errorString(teError eError);
teError      errorCode(const char *pString);

#ifdef __cplusplus
}
#endif


#endif // ERROR_H
