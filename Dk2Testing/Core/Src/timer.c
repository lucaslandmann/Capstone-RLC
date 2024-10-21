///////////////////////////////////////////////////////////////////////////////
//
// @file timer.c
// @brief This file contains the timer implementation.
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
typedef struct
{
	bool			   periodic;
	uint32_t           interval;
	volatile uint32_t  remaining;
	tpTimerCallback    pCallback;
	void              *pContext;
} trTimerCallback;

// P R I V A T E  D A T A
static volatile uint64_t grTimerMilliseconds = 0;

static volatile trTimerCallback grTimerCallback[TIMER_CALLBACKS] = {0};

// P R I V A T E  F U N C T I O N S
//
// @brief Tick handler.
//
// @param pContext pointer to context data
// @return none
//
static void tickHandler(void *pContext)
{
	int index;

	// Increment millisecond counter.
	grTimerMilliseconds++;

	// Iterate over callbacks.
	for (index = 0; index < DIM(grTimerCallback); index++)
	{
		// Check entry.
		if (NULL != grTimerCallback[index].pCallback)
		{
			// Update remaining time.
			if (grTimerCallback[index].remaining > 0)
			{
				grTimerCallback[index].remaining--;
			}
		}
	}
}

// P U B L I C  F U N C T I O N S
//
// @brief Timer initialization.
//
// @return enumerated error code.
//
teError timerInit(void)
{
	// Initialize timer callbacks.
	memset((void*)grTimerCallback, 0, sizeof(grTimerCallback));

	// Install timer tick callback.
	bspTickHandler(tickHandler, NULL);

	return eErrorNone;
}

//
// @brief Retrieve the number of milliseconds since boot.
//
// @return milliseconds
//
uint64_t timerMilliseconds(void)
{
	return grTimerMilliseconds;
}

//
// @brief Retrive timestamp.
//
// This function converts the global timer tick count to the number of days,
// hours, minutes, seconds, and milliseconds of operation.
//
// @param prTimestamp pointer to timestamp data structure.
// @return void
//
void timerTimestamp(trTimerTimestamp *prTimestamp)
{
	uint64_t uptime;

    // Validate argument[s].
    ASSERT(NULL != prTimestamp);

    // Get total uptime in milliseconds.
	uptime = timerMilliseconds();
	prTimestamp->seconds = uptime / 1000;

	// Calculate uptime units.
	prTimestamp->days         =  (uptime / 1000) / (60 * 60 * 24);
	prTimestamp->hours        = ((uptime / 1000) / (60 * 60)) % 24;
	prTimestamp->minutes      = ((uptime / 1000) / 60) % 60;
	prTimestamp->seconds      =  (uptime / 1000) % 60;
	prTimestamp->milliseconds =   uptime % 1000;
}

//
// @brief Add timer callback function.
//
// @param interval period time in units of milliseconds
// @param pCallback pointer to callback function
// @param pContext pointer to callback context
// @param periodic true = periodic, false = one-shot
// @return enumerated error code
//
teError timerAdd(uint32_t interval, tpTimerCallback pCallback,
					     void *pContext, bool periodic)
{
	int index;

	// Validate argument[s].
	ASSERT(NULL != pCallback);

	// Find callback resource.
	for (index = 0; index < DIM(grTimerCallback); index++)
	{
		// Check entry.
		if (NULL == grTimerCallback[index].pCallback)
		{
			// Critical section begin.
			bspInterruptEnable(false);

			// Initialize entry.
			grTimerCallback[index].periodic  = periodic;
			grTimerCallback[index].interval  = interval;
			grTimerCallback[index].remaining = interval;
			grTimerCallback[index].pCallback = pCallback;
			grTimerCallback[index].pContext  = pContext;

			// Critical section end.
			bspInterruptEnable(true);

			return eErrorNone;
		}
	}
	LOGERROR(eErrorOutOfResources);

	return eErrorOutOfResources;
}

//
// @brief Delete timer callback function.
//
// @param pCallback pointer to callback function
// @return enumerated error code
//
teError timerDelete(tpTimerCallback pCallback)
{
	int index;

	// Validate argument[s].
	ASSERT(NULL != pCallback);

	// Find callback resource.
	for (index = 0; index < DIM(grTimerCallback); index++)
	{
		// Check entry.
		if (pCallback == grTimerCallback[index].pCallback)
		{
			// Critical section begin.
			bspInterruptEnable(false);

			// Clear entry.
			memset((void*)&grTimerCallback[index], 0,
                sizeof(grTimerCallback[index]));

			// Critical section end.
			bspInterruptEnable(true);

			return eErrorNone;
		}
	}
	LOGERROR(eErrorNotFound);

	return eErrorNotFound;
}

//
// @brief Process pending timer callback[s].
//
// @return none
//
void timerService(void)
{
	int index;

	// Iterate over timer callbacks.
	for (index = 0; index < DIM(grTimerCallback); index++)
	{
		// Check entry.
		if (NULL != grTimerCallback[index].pCallback)
		{
			// Check remaining time.
			if (0 == grTimerCallback[index].remaining)
			{
				// Invoke callback.
				grTimerCallback[index].pCallback(grTimerCallback[index].pContext);

				// Check periodicity.
				if (grTimerCallback[index].periodic)
				{
					// Reset interval.
					grTimerCallback[index].remaining = grTimerCallback[index].interval;
				}
				else
				{
					// Delete entry.
					timerDelete(grTimerCallback[index].pCallback);
				}

			}
		}
	}
}

