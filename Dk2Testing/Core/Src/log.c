///////////////////////////////////////////////////////////////////////////////
//
// @file log.c
// @brief This file contains the log implementation.
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

// P R I V A T E  D A T A
static teLogLevel    geLogLevel = eLogLevelWarn;
static trLogError    grLogErrorHistory[LOG_ERROR_HISTORY_DEPTH];
static uint16_t		 gLogErrorHistoryHead = 0;

// P R I V A T E  F U N C T I O N S
//
// @brief Retrieve pointer to base file name.
//
// @param pFilename pointer to null terminated string
// @return pointer to null terminated string
//
static char * logFilename(const char *pFilename)
{
    char *pString;

    // Validate argument[s].
    ASSERT(NULL != pFilename);

    // Scan for last forward slash if present.
    pString = (char*)pFilename + strlen(pFilename);
    while (pString != pFilename)
    {
        // Check for forward slash.
        if ('/' == *(pString - 1))
        {
            return pString;
        }

        // Move to previous character.
        pString--;
    }

    return pString;
}

//
// @brief Print message prefix to specified I/O device.
//
// @param eLogLevel enumerated log level
// @param pFilename pointer to null terminated string containing the file name
// @param line line number
// @param pFunction pointer to null terminated string containing the function
//                  name
// @return none
//
static void logPrefix(teLogLevel eLogLevel, const char *pFilename,
			   	      const uint16_t line, const char *pFunction,
					  char *pBuffer, uint16_t length)
{
	trTimerTimestamp   rTimestamp;
    static const char *pPrefix[] = { "NONE", "FATAL", "ASSERT", "ERR",
    								 "WARN", "INFO", "DBG", "MAX" };

    // Validate arguments.
    ASSERT(NULL != pFilename);
    ASSERT(line > 0);
    ASSERT(NULL != pFunction);
    ASSERT(eLogLevel <= eLogLevelMax);
    ASSERT(NULL != pBuffer);
    ASSERT(length > 0);

    // Format and log string.
    timerTimestamp(&rTimestamp);
    snprintf(pBuffer, length, "%s[%d.%02u:%02u:%02u.%03u,%s,%u,%s]: ",
        pPrefix[eLogLevel], rTimestamp.days, rTimestamp.hours, rTimestamp.minutes,
        rTimestamp.seconds, rTimestamp.milliseconds, logFilename(pFilename),
        line, pFunction);
}

// P U B L I C  F U N C T I O N S
//
// @brief Initialize log.
//
// @return enumerated error code
//
teError logInit(void)
{
	// Initialize history buffer.
	memset(grLogErrorHistory, 0, sizeof(grLogErrorHistory));

	return eErrorNone;
}

//
// @brief Get current level of active logging.
//
// @return enumerated log level
//
teLogLevel logLevelGet(void)
{
    return geLogLevel;
}

//
// @brief Set current level of active logging.
//
// @param eLogLevel enumerated log level
// @return void
//
void logLevelSet(teLogLevel eLogLevel)
{
    geLogLevel = (eLogLevel < eLogLevelNone) ? eLogLevelNone : eLogLevel;
    geLogLevel = (eLogLevel > eLogLevelMax)  ? eLogLevelMax  : eLogLevel;
}

//
// @brief Defensive action for run-time assumptions.
//
// @param bIgnore condition state
// @param pFilename pointer to null terminated string containing the file name
// @param line line number
// @param pFunction pointer to null terminated string containing the function
//                  name
// @param pDescription pointer to null terminated string containing a
//                     description
// @return void
//
void logAssert(bool bIgnore, const char *pFilename,
    const uint16_t line, const char *pFunction,
    const char *pDescription)
{
    bool state = true;

    // Check condition.
    if (!bIgnore)
    {
        // Display location information.
    	logEntry(eLogLevelAssert, pFilename, line, pFunction, pDescription);

        // Flush the console.
        bspConsoleFlush();

        // Spin forever!
        for (;;)
        {
            // Set heartbeat LED to indicate fault.
            bspHeartbeatLed(state = !state);

            // Wait for a bit.
            bspMillisecondDelay(BSP_FAULT_LED_FLASH_PERIOD);
        }
    }
}

//
// @brief Create log entry.
//
// @param eLogLevel enumerated log level
// @param pFilename pointer to null terminated string containing the file name
// @param line line number
// @param pFunction pointer to null terminated string containing the function
//                  name
// @param pFormat pointer to null terminated string containing the format
//				  specifier
// @param ... variable argument list
// @return void
//
void logEntry(teLogLevel eLogLevel, const char *pFilename,
    const uint16_t line, const char *pFunction,
    const char *pFormat, ...)
{
    va_list args;
    char    buffer[LOG_LINE_LENGTH_MAX];

    // Filter log level.
    if (eLogLevel > geLogLevel)
    {
        return;
    }

    // Validate argument[s].
    ASSERT(NULL != pFilename);
    ASSERT(line > 0);
    ASSERT(NULL != pFunction);

    // Print prefix information.
    logPrefix(eLogLevel, pFilename, line, pFunction, buffer, sizeof(buffer));
    bspPrintf("\r\n%s", buffer);

    // Print formatted string.
	//lint -save -e530
	va_start(args, pFormat);
	//lint -restore
	vsnprintf(buffer, sizeof(buffer), pFormat, args);
	va_end(args);
	bspPrintf(buffer);
}

//
// @brief Create log data entry.
//
// @param eLogLevel enumerated log level
// @param pFilename pointer to null terminated string containing the file name
// @param line line number
// @param pFunction pointer to null terminated string containing the function
//                  name
// @param pTitle pointer to null terminated string containing the title
// @param pBuffer pointer to data
// @param length length of data in bytes
// @return void
//
void logDataEntry(teLogLevel eLogLevel, const char *pFilename,
    const uint16_t line, const char *pFunction,
    const char *pTitle, void *pBuffer, uint32_t length)
{
    uint32_t  index;
    uint8_t  *pData = (uint8_t*)pBuffer;
    char      buffer[LOG_LINE_LENGTH_MAX];

    // Filter log level.
    if (eLogLevel > geLogLevel)
    {
        return;
    }

    // Validate argument[s].
    ASSERT(NULL != pFilename);
    ASSERT(line > 0);
    ASSERT(NULL != pFunction);
    ASSERT(NULL != pBuffer);
    ASSERT(length > 0);

    // Display location information.
    logPrefix(eLogLevel, pFilename, line, pFunction, buffer, sizeof(buffer));
    bspPrintf("\r\n%s", buffer);

	// Print title if present.
	if (NULL != pTitle)
	{
		bspPrintf("%s, ", pTitle);
	}

	// Print address and length.
	bspPrintf("%08p, %lu bytes", pBuffer, length);

	// Print buffer contents.
	for (index = 0; index < length; index++)
	{
		if (0 == index % 16)
		{
			bspPrintf("\r\n%08p: ",pData + index);
		}
		bspPrintf("%02x ", pData[index]);
	}
}

//
// @brief Create log entry based on enumerated error code.
//
// @param eError enumerated error code
// @param pFilename pointer to filename string
// @param line source code line number
// @param pFunction pointer to function name string
// @return enumerated error code
teError logError(teError eError, const char *pFilename,
    const uint16_t line, const char *pFunction)
{
    // Validate argument[s].
    ASSERT(NULL != pFilename);
    ASSERT(NULL != pFunction);

    // Advance head.
    gLogErrorHistoryHead = (gLogErrorHistoryHead + 1)%DIM(grLogErrorHistory);

    // Assign error entry.
    grLogErrorHistory[gLogErrorHistoryHead].eError = eError;
    strncpy(grLogErrorHistory[gLogErrorHistoryHead].file,
        logFilename(pFilename), sizeof(grLogErrorHistory[0].file) - 1);
    grLogErrorHistory[gLogErrorHistoryHead].line = line;

    // Issue log entry.
    logEntry(eLogLevelError, pFilename, line, pFunction, errorString(eError));

    return eErrorNone;
}

//
// @brief Defensive action for run-time failures.
//
// @param eError enumerated error code
// @param pFilename pointer to null terminated string containing the file name
// @param line line number
// @param pFunction pointer to null terminated string containing the function
//                  name
// @return none
//
void logFatal(teError eError, const char *pFilename,
    const uint16_t line, const char *pFunction)
{
    bool state = true;

    // Advance head.
    gLogErrorHistoryHead = (gLogErrorHistoryHead + 1)%DIM(grLogErrorHistory);

    // Assign error entry.
    grLogErrorHistory[gLogErrorHistoryHead].eError = eError;
    strncpy(grLogErrorHistory[gLogErrorHistoryHead].file,
        logFilename(pFilename), sizeof(grLogErrorHistory[0].file) - 1);
    grLogErrorHistory[gLogErrorHistoryHead].line = line;

    // Issue log entry.
    logEntry(eLogLevelFatal, pFilename, line, pFunction, errorString(eError));

    // Flush the console.
    bspConsoleFlush();

    // Spin forever!
    for (;;)
    {
        // Set heartbeat LED to indicate fault.
        bspHeartbeatLed(state = !state);

        // Wait for a bit.
        bspMillisecondDelay(BSP_FAULT_LED_FLASH_PERIOD);
    }
}

//
// @brief Retrieve error history string.
//
// @param pBuffer pointer to buffer
// @param length size of buffer in bytes
// @return true = content, false = empty
//
bool logErrorHistory(trLogError *prError)
{
	// Validate argument[s].
	ASSERT(NULL != prError);

	// Check content.
	if (eErrorNone != grLogErrorHistory[gLogErrorHistoryHead].eError)
	{
		// Copy and clear entry.
	    memcpy(prError, &grLogErrorHistory[gLogErrorHistoryHead],
            sizeof(*prError));
		memset(&grLogErrorHistory[gLogErrorHistoryHead], 0,
            sizeof(grLogErrorHistory[0]));
    	gLogErrorHistoryHead = (gLogErrorHistoryHead + 1)%DIM(grLogErrorHistory);

    	return true;
	}

	return false;
}
