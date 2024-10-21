///////////////////////////////////////////////////////////////////////////////
//
// @file perf.c
// @brief This file contains the performance implementation.
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
static trPerf grPerf[PERF_MAX] = {0};

// P R I V A T E  F U N C T I O N S
//
// @brief Print performance stats.
//
// @return enumerated error code
//
static void dump(trPerf *prPerf)
{
    // Validate argument[s].
    ASSERT(NULL != prPerf);

    // Print stats.
    bspPrintf("\r\n%s(min/max/avg): %1.6E/%1.6E/%1.6E", prPerf->name,
        (double)prPerf->rMeasure.min / (double)bspCycleCounterFrequency(),
        (double)prPerf->rMeasure.max / (double)bspCycleCounterFrequency(),
        (double)prPerf->rMeasure.avg / (double)bspCycleCounterFrequency());
}

// P U B L I C  F U N C T I O N S
//
// @brief Initialize performance module.
//
// @return enumerated error code
//
teError perfInit(void)
{
    // Initialize performance resources.
    memset(grPerf, 0, sizeof(grPerf));

    return eErrorNone;
}

//
// @brief Create performance instance.
//
// @param pprPerf indirect pointer to performance structure
// @param pName pointer to null terminated string name
// @return enumerated error code
//
teError perfCreate(trPerf **pprPerf, const char *pName)
{
    int index;

    // Validate argument[s].
    ASSERT(NULL != pprPerf);
    ASSERT(NULL != pName);

    // Iterate over resources.
    for (index = 0; index < DIM(grPerf); index++)
    {
        // Check slot allocation.
        if (!grPerf[index].allocated)
        {
            // Allocate, copy name, and assign reference.
            grPerf[index].allocated = true;
            strncpy(grPerf[index].name, pName, sizeof(grPerf[index].name));
            *pprPerf = &grPerf[index];

            // Initialize stats.
            grPerf[index].rMeasure.min = UINT32_MAX;
            grPerf[index].rMeasure.max = 0;

            return eErrorNone;
        }
    }

    return eErrorOutOfResources;
}

//
// @brief Destroy performance instance.
//
// @param prPerf pointer to performance structure
// @return enumerated error code
//
teError perfDestroy(trPerf *prPerf)
{
    int index;

    // Validate argument[s].
    ASSERT(NULL != prPerf);

    // Iterate over resources.
    for (index = 0; index < DIM(grPerf); index++)
    {
        // Check pointer.
        if (&grPerf[index] == prPerf)
        {
            // Clear slot.
            memset(&grPerf[index], 0, sizeof(grPerf[index]));

            return eErrorNone;
        }
    }

    return eErrorArgument;
}

//
// @brief Pring performance instance.
//
// @param prPerf optional pointer to performance structure
// @return enumerated error code
//
void perfPrint(trPerf *prPerf)
{
    int index;

    // Check pointer.
    if (NULL != prPerf)
    {
        dump(prPerf);

        return;
    }

    // Iterate over resources.
    for (index = 0; index < DIM(grPerf); index++)
    {
        // Check allocation.
        if (grPerf[index].allocated)
        {
            dump(&grPerf[index]);
        }
    }
}

//
// @brief Capture performance instance start time.
//
// @param prPerf optional pointer to performance structure
// @return enumerated error code
//
void perfStart(trPerf *prPerf)
{
    // Validate argument[s].
    ASSERT(NULL != prPerf);

    // Capture cycle count.
    prPerf->start = bspCycleCounter();
}

//
// @brief Capture performance instance stop time.
//
// @param prPerf optional pointer to performance structure
// @return enumerated error code
//
void perfStop(trPerf *prPerf)
{
    uint32_t delta;

    // Validate argument[s].
    ASSERT(NULL != prPerf);

    // Capture cycle count.
    prPerf->stop = bspCycleCounter();

    // Compute stats.
    delta                = (prPerf->stop > prPerf->start) ?
                            prPerf->stop - prPerf->start :
                            (UINT32_MAX - prPerf->stop) + prPerf->start;
    prPerf->rMeasure.min = (prPerf->rMeasure.min > delta) ? delta :
                            prPerf->rMeasure.min;
    prPerf->rMeasure.max = (prPerf->rMeasure.max < delta) ? delta :
                            prPerf->rMeasure.max;
    prPerf->rMeasure.avg = (delta + prPerf->rMeasure.avg) >> 1;
}
