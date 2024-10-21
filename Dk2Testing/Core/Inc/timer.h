///////////////////////////////////////////////////////////////////////////////
//
// @file timer.h
// @brief This file contains the timer interface definition.
//
//  COPYRIGHT (c) 2022 Summit Scientific, Inc. All rights reserved.
//  NOTICE:  Contains confidential and proprietary information of
//           Summit Scientific, Inc. and is protected under the copyright
//           laws as an unpublished work.
//
// @author R. Van Rooyen
//
///////////////////////////////////////////////////////////////////////////////

#ifndef TIMER_H
#define TIMER_H

// D E F I N E S
#define TIMER_CALLBACKS				(8)
#define TIMER_TICKS_PER_SECOND      (1000)

// T Y P E D E F S
typedef void (*tpTimerCallback)(void *pContext);

// S T R U C T U R E S
typedef struct
{
    uint16_t days;
    uint8_t  hours;
    uint8_t  minutes;
    uint8_t  seconds;
    uint16_t milliseconds;
} trTimerTimestamp;

// P U B L I C  F U N C T I O N S
#ifdef __cplusplus
extern "C" {
#endif

teError  timerInit(void);
void     timerTimestamp(trTimerTimestamp *prTimestamp);
uint64_t timerMilliseconds(void);
teError  timerAdd(uint32_t interval, tpTimerCallback pCallback, void *pContext, bool periodic);
teError  timerDelete(tpTimerCallback pCallback);
void     timerService(void);

#ifdef __cplusplus
}
#endif

#endif // TIMER_H
