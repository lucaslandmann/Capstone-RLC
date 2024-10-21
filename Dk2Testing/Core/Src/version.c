///////////////////////////////////////////////////////////////////////////////
//
// @file version.c
// @brief This file contains the version implementation.
//
//  COPYRIGHT (c) Summit Scientific, Inc. All rights reserved.
//  NOTICE:  Contains confidential and proprietary information of
//           Summit Scientific, Inc. and is protected under the copyright
//           laws as an unpublished work.
//
// @author R. Van Rooyen
//
///////////////////////////////////////////////////////////////////////////////

// P R O J E C T  I N C L U D E S
#include "common.h"
#include "version.h"

// P U B L I C  F U N C T I O N S
//
// @brief Convert confidence to string.
//
// @param confidence value
// @return pointer to null terminated string
//
const char * versionConfidence(uint16_t confidence)
{
    static const char *pConfidence[] = { "omega", "alpha", "beta", "release" };

    // Validate argument[s].
    ASSERT(confidence < DIM(pConfidence));

    return pConfidence[confidence];
}
