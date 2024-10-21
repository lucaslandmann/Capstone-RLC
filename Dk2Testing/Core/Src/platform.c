///////////////////////////////////////////////////////////////////////////////
//
// @file platform.c
// @brief This file contains the platform implementation.
//
//  COPYRIGHT (c) Summit Scientific, Inc. All rights reserved.
//  NOTICE:  Contains confidential and proprietary information of
//           Summit Scientific, Inc. and is protected under the copyright
//           laws as an unpublished work.
//
// @author R. Van Rooyen
//
///////////////////////////////////////////////////////////////////////////////

// S Y S T E M  I N C L U D E S
#include <stdint.h>
#include <stdbool.h>

// P R O J E C T  I N C L U D E S
#include "common.h"
#include "main.h"
#include "version.h"
#include "platform.h"
#include "pcm6260.h"
#include "mixer.h"

// P R I V A T E  F U N C T I O N S
//
// @brief Hearbeat LED callback function.
//
// @param pContext pointer to callback context
// @return none
//
static void heartbeatHandler(void *pContext)
{
	static bool state = false;

	// Toggle LED.
	bspHeartbeatLed(state = !state);
}

//
// @brief Print version information.
//
// @param argc argument count
// @param argv pointer to array of pointers to null terminated argument strings
// @return enumerated error code
//
static teError version(int argc, char *argv[])
{
	// Print version.
    bspPrintf("\r\n%u.%u.%u %s", VERSION_MAJOR, VERSION_MINOR,
		VERSION_BUILD, versionConfidence(VERSION_CONF));

    return eErrorNone;
}

// P U B L I C  F U N C T I O N S
//
// @brief Intialize the platform.
//
// @param prBsp pointer to BSP data.
// @return true = success, false = failure
//
bool platformInit(trBsp *prBsp)
{
	teError eError;

	// Initialize BSP.
	if (!bspInit(prBsp))
	{
		return false;
	}

	// Disable I/O buffering.
	setvbuf(stdout, NULL, _IONBF, 0);

	// Initialize ADC.
	if (eErrorNone != (eError = pcm6260Init()))
	{
		LOGFATAL(eError);
	}

    // Initialize performance.
    if (eErrorNone != (eError = perfInit()))
    {
        LOGFATAL(eError);
    }

	// Initialize timer.
	if (eErrorNone != (eError = timerInit()))
	{
		LOGFATAL(eError);
	}

	// Initialize log.
	if (eErrorNone != (eError = logInit()))
	{
		LOGFATAL(eError);
	}

	// Initialize mixer.
	/*
	if (eErrorNone != (eError = mixerInit()))
	{
		LOGFATAL(eError);
	}
	*/

	// Initialize shell.
	if (eErrorNone != (eError = shellInit()))
	{
		LOGFATAL(eError);
	}

    // Add shell commands.
    if (eErrorNone != (eError = shellAdd("ver",
        "version information", "", "", 1, version)))
    {
        LOGERROR(eError);
    }

	// Add LED heartbeat callback.
	if (eErrorNone != (eError = timerAdd(PLATFORM_HEARTBEAT_INTERVAL,
		heartbeatHandler, NULL, true)))
	{
		LOGFATAL(eError);
	}

	// Clear terminal.
	puts(VT100_CLEAR_SCREEN);
	puts(VT100_CURSOR_HOME);

    // Print shell banner.
    bspPrintf("\r\n\n%s %s Version %d.%d.%d %s (%s)" , PLATFORM_MDL,
		PLATFORM_DESCRIPTION, VERSION_MAJOR, VERSION_MINOR,
		VERSION_BUILD, versionConfidence(VERSION_CONF), PLATFORM_TYPE);
    bspPrintf("\r\n%s %s %s" ,PLATFORM_COPYRIGHT, PLATFORM_MFG_FULL,
		PLATFORM_RIGHTS);

    // Wait for power rails to stabilize.
    bspMillisecondDelay(BSP_WAIT_RAIL_STABLIZE);

    // TODO: Perform self-test.

	return true;
}

//
// @brief Platform thread of execution.
//
// @return true = success, false = failure
//
bool platformService(void)
{
	teError eError;

    // Service timers.
    timerService();

	// Service diagnostic shell.
	if (eErrorNone != (eError = shellService()))
	{
		LOGERROR(eError);
	}

	// Service mixer.
	/*
	if (eErrorNone != (eError = mixerService()))
	{
		LOGERROR(eError);
	}
	*/

    // Try to drain stdout.
    bspConsoleDrain();

	return true;
}

//
// @brief Shutdown the platform.
//
// @param none
// @return none
//
void platformShutdown(void)
{
	// Log event.
	LOGFATAL(eErrorUnknown);
}
