///////////////////////////////////////////////////////////////////////////////
//
// @file bsp.h
// @brief This file contains the board support package interface definition.
//
//  COPYRIGHT (c) 2022 Summit Scientific, Inc. All rights reserved.
//  NOTICE:  Contains confidential and proprietary information of
//           Summit Scientific, Inc. and is protected under the copyright
//           laws as an unpublished work.
//
// @author R. Van Rooyen
//
///////////////////////////////////////////////////////////////////////////////

#ifndef BSP_H
#define BSP_H

// S Y S T E M  I N C L U D E S
#include <stdbool.h>

// P R O J E C T  I N C L U D E S
#include "error.h"
#include "ring.h"

// D E F I N E S
#define BSP_CPU_CLOCK_FREQUENCY                 (128E+06)

#define BSP_STDIN_BUFFER_SIZE	                (1024)
#define BSP_STDOUT_BUFFER_SIZE	                (4096)
#define BSP_SPI_BUFFER_SIZE                     (4)
#define BSP_PRINTF_BUFFER_SIZE                  (80)

#define BSP_UART_TIMEOUT                        (10)
#define BSP_SPI_TIMEOUT                         (100)
#define BSP_I2C_TIMEOUT                         (100)

#define BSP_I2C_BUFFER_MAX                      (4)

#define BSP_FAULT_LED_FLASH_PERIOD              (100)

#define BSP_PRESS_ANY_KEY_ACTIVITY_INTERVAL     (1000)

#define BSP_WAIT_RAIL_STABLIZE                  (500)

// T Y P E D E F S
typedef void (*tpBspTickHandler)(void *pContext);

// S T R U C T U R E S
typedef struct _trBsp
{
	// HAL objects.
    void *pUart1;
	void *pI2c1;
	void *pSaiBlockB2;
	void *pAdc1;
	void *pAdc4;

	// Tick handler.
    tpBspTickHandler  pTickHandler;
	void             *pTickContext;

	// Character and I/O ring buffers.
	char   stdRx;
	thRing hStdin;
	thRing hStdout;

} trBsp;

// P U B L I C  F U N C T I O N S
#ifdef __cplusplus
extern "C" {
#endif

trBsp* 			bsp(void);
bool            bspConsolePressAnyKey(char *pMessage, uint16_t timeout);
bool            bspConsoleEmpty(void);
void            bspConsoleDrain(void);
void            bspConsoleFlush(void);
bool            bspConsoleRead(char *pCh);
void            bspConsoleWrite(char ch);
uint32_t        bspCycleCounter(void);
uint32_t        bspCycleCounterFrequency(void);
void            bspMicrosecondDelay(uint32_t microseconds);
void            bspMillisecondDelay(uint32_t milliseconds);
void            bspNanosecondDelay(uint32_t nanoseconds);
void            bspHeartbeatLed(bool state);
bool            bspI2cRead(uint8_t address, uint8_t *pBuffer, uint16_t length);
bool            bspI2cRegRead(uint8_t address, uint16_t reg, uint16_t size,
                              uint8_t *pBuffer, uint16_t length);
bool            bspI2cRegWrite(uint8_t address, uint16_t reg, uint16_t size,
                               uint8_t *pBuffer, uint16_t length);
bool            bspI2cWrite(uint8_t address, uint8_t *pBuffer,
                            uint16_t length);
bool            bspInit(trBsp *prBsp);
void            bspInterruptEnable(bool state);
int             bspPrintf(const char *pFormat, ...);
void            bspReset(void);
teError         bspSpiSelect(uint8_t channel, bool state);
teError         bspSpiTransfer(uint8_t channel, uint8_t *pTxBuffer,
                               uint8_t *pRxBuffer, uint16_t length,
                               bool select);
void            bspTick(void);
void            bspTickHandler(tpBspTickHandler pHandler, void *pContext);

#ifdef __cplusplus
}
#endif

#endif // BSP_H
