///////////////////////////////////////////////////////////////////////////////
//
// @file error.c
// @brief This file contains the error implementation.
//
//  COPYRIGHT (c) 2022 Summit Scientific, Inc. All rights reserved.
//  NOTICE:  Contains confidential and proprietary information of
//           Summit Scientific, Inc. and is protected under the copyright
//           laws as an unpublished work.
//
// @author R. Van Rooyen
//
///////////////////////////////////////////////////////////////////////////////

// P R O J E C T  I N C L U D E S
#include "common.h"

// S T R U C T U R E S
typedef struct _trError
{
    teError 	 eError;
    const char * pDescription;
} trError;

// P R I V A T E  D A T A
//
// NOTE: Error table must be in sync with enumerated error codes in error.h.
//
static trError grError[] =
{
    { eErrorNone,			"none" },
    { eErrorAbort,          "abort" },
    { eErrorAddress,		"address" },
    { eErrorArgument,		"argument" },
    { eErrorAuthenticate,   "authenticate" },
    { eErrorChannel,        "channel" },
    { eErrorCrc,            "crc" },
    { eErrorDone,           "done" },
    { eErrorEmpty,			"empty" },
    { eErrorErase,          "erase" },
    { eErrorFault,          "fault" },
    { eErrorFull,           "full" },
    { eErrorHash,           "hash" },
    { eErrorInit,			"init" },
    { eErrorInvalid,		"invalid" },
    { eErrorLength,			"length" },
    { eErrorMagic,          "magic" },
    { eErrorNotFound,		"not-found" },
    { eErrorOutOfResources,	"out-of-resources" },
    { eErrorParameter,		"parameter" },
    { eErrorParse,			"parse" },
    { eErrorProgram,        "program" },
    { eErrorReadoutLevel,   "read-out-level" },
    { eErrorRead,           "read" },
    { eErrorReceive,		"receive" },
    { eErrorSend,			"send" },
    { eErrorSelfTest,		"self-test" },
    { eErrorStart,			"start" },
    { eErrorState,          "state" },
    { eErrorStop,			"stop" },
    { eErrorTemperature,    "temperature" },
    { eErrorTimeout,		"timeout" },
    { eErrorType,			"type" },
    { eErrorUnsupported, 	"unsupported" },
    { eErrorUnknown, 		"unknown" },
    { eErrorVerify,         "verify" },
    { eErrorVersion,        "version" },
    { eErrorVoltage,        "voltage" },
    { eErrorWrite,          "write" },
    { eErrorWriteProtect,   "write-protect" }
};

// P U B L I C  F U N C T I O N S
//
// @brief Map error code to string.
//
// @param eError enumerated error value
// @return enumerated error code
//
const char * errorString(teError eError)
{
    // Validate argument[s].
    ASSERT(eError < DIM(grError));
    ASSERT(eError == grError[eError].eError);

    return grError[eError].pDescription;
}

//
// @brief Map error string to code.
//
// @param pString pointer to null terminated string
// @return enumerated error code
//
teError errorCode(const char *pString)
{
    int index;

    // Validate argument[s].
    ASSERT(NULL != pString);

    // Iterate over error table.
    for (index = 0; index < DIM(grError); index++)
    {
        // Check for match.
        if (0 == strcmp(pString, grError[index].pDescription))
        {
            return grError[index].eError;
        }
    }

    return eErrorNone;
}

