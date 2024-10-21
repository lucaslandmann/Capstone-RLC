///////////////////////////////////////////////////////////////////////////////
//
// @file perf.h
// @brief This file contains the performance interface definition.
//
//  COPYRIGHT (c) 2022 Summit Scientific, Inc. All rights reserved.
//  NOTICE:  Contains confidential and proprietary information of
//           Summit Scientific, Inc. and is protected under the copyright
//           laws as an unpublished work.
//
// @author R. Van Rooyen
//
///////////////////////////////////////////////////////////////////////////////

#ifndef PERF_H
#define PERF_H

// D E F I N E S
#define PERF_MAX                (8)
#define PERF_STRING_MAX         (32)

// S T R U C T U R E S
typedef struct _trPerf
{
    // Attributes.
    bool allocated;
    char name[PERF_STRING_MAX];

    // Time.
    uint32_t start;
    uint32_t stop;

    // Stats.
    struct
    {
        uint32_t min;
        uint32_t max;
        uint32_t avg;
    } rMeasure;
} trPerf;

// P U B L I C  F U N C T I O N S
#ifdef __cplusplus
extern "C" {
#endif

teError perfCreate(trPerf **pprPerf, const char *pName);
teError perfDestroy(trPerf *prPerf);
teError perfInit(void);
void    perfPrint(trPerf *prPerf);
void    perfStart(trPerf *prPerf);
void    perfStop(trPerf *prPerf);

#ifdef __cplusplus
}
#endif

#endif // PERF_H
