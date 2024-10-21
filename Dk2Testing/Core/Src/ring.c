///////////////////////////////////////////////////////////////////////////////
//
// @file ring.c
// @brief This file contains the ring implementation.
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
typedef volatile struct _trRing
{
	uint32_t  head;
	uint32_t  tail;
	uint32_t  length;
	char     *pBuffer;
} trRing;

// P R I V A T E  F U N C T I O N S
//
// @brief Determine empty state of ring buffer.
//
// @param prRing pointer to ring structure
// @return true = empty, false = not empty
//
static inline bool empty(trRing *prRing)
{
    // Validate argument[s].
    ASSERT(NULL != prRing);

    return (prRing->head == prRing->tail) ? true : false;
}

//
// @brief Determine if ring buffer is full.
//
// @param trRing pointer to ring structure
// @return true = full, false = not full
//
static inline bool full(trRing *prRing)
{
    // Validate argument[s].
    ASSERT(NULL != prRing);

    return ((prRing->head + 1) % prRing->length == prRing->tail) ? true : false;
}

//
// @brief Determine length of free space in ring.
//
// @param thRing handle to ring structure
// @return length of free space
//
static inline uint32_t space(trRing *prRing)
{
    // Validate argument[s].
    ASSERT(NULL != prRing);

    // Check empty state.
    if (empty(prRing))
    {
        return prRing->length;
    }

    return (prRing->head > prRing->tail) ?
            prRing->length - prRing->head + prRing->tail - 1 :
            prRing->tail - prRing->head;
}


// P U B L I C  F U N C T I O N S
//
// @brief Initialize ring buffer.
//
// @param pBuffer pointer to buffer.
// @param length length of buffer in bytes
// @return handle to ring
//
thRing ringInit(char *pBuffer, uint32_t length)
{
	trRing *prRing = (trRing*)pBuffer;

	// Validate argument[s].
	ASSERT(NULL != prRing);
	ASSERT(length > 0);

	// Initialize ring.
	memset((void*)prRing, 0, length);
	prRing->length  = length - sizeof(*prRing);
	prRing->pBuffer = pBuffer + sizeof(*prRing);

	return (thRing)prRing;
}

//
// @brief Determine empty state of ring buffer.
//
// @param hRing handle to ring structure
// @return true = empty, false = not empty
//
bool ringEmpty(thRing hRing)
{
	trRing *prRing = (trRing*)hRing;

	// Validate argument[s].
	ASSERT(NULL != prRing);

	return empty(prRing);
}

//
// @brief Determine if ring buffer is full.
//
// @param thRing handle to ring structure
// @return true = full, false = not full
//
bool ringFull(thRing hRing)
{
    trRing *prRing = (trRing*)hRing;

    // Validate argument[s].
    ASSERT(NULL != prRing);

    return full(prRing);
}

//
// @brief Determine length of free space in ring.
//
// @param thRing handle to ring structure
// @return length of free space
//
uint32_t ringSpace(thRing hRing)
{
    trRing *prRing = (trRing*)hRing;

    // Validate argument[s].
    ASSERT(NULL != prRing);

    return space(prRing);
}

//
// @brief Write buffer of specified length to ring buffer.
//
// @param hRing pointer to ring structure
// @param pBuffer pointer to buffer.
// @param length length of buffer in bytes
// @return number of bytes written
//
uint32_t ringWrite(thRing hRing, const char *pBuffer, const uint32_t length)
{
	uint32_t  bytes  = 0;
	trRing   *prRing = (trRing*)hRing;

	// Validate argument[s].
	ASSERT(NULL != prRing);

    // Check free space.
    if (space(prRing) >= length)
    {
        // Write buffer to ring.
        while (!full(prRing) && (bytes < length))
        {
            // Write character.
            prRing->pBuffer[prRing->head] = pBuffer[bytes++];

            // Update head index.
            prRing->head = (prRing->head + 1) % prRing->length;
        }
    }

	return bytes;
}

//
// @brief Read specified length or less from ring buffer.
//
// @param hRing pointer to ring structure
// @param pBuffer pointer to buffer.
// @param length length of buffer in bytes
// @return number of bytes read
//
uint32_t ringRead(thRing hRing, char *pBuffer, const uint32_t length)
{
	uint32_t  bytes  = 0;
	trRing   *prRing = (trRing*)hRing;

	// Validate argument[s].
	ASSERT(NULL != prRing);
	ASSERT(length > 0);

	// Read buffer from ring.
	while (!empty(prRing) && (bytes < length))
	{
		// Read character.
		pBuffer[bytes++] = prRing->pBuffer[prRing->tail];

		// Update tail index.
		prRing->tail = (prRing->tail + 1) % prRing->length;
	}

	return bytes;
}

