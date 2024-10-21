///////////////////////////////////////////////////////////////////////////////
//
// @file bsp.c
// @brief This file contains the board support package implementation.
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
#include "main.h"

// P R I V A T E  D A T A
// BSP
static trBsp *gprBsp = NULL;

// I/O
static char   gStdinBuffer[BSP_STDIN_BUFFER_SIZE]     = {0};
static char   gStdoutBuffer[BSP_STDOUT_BUFFER_SIZE]   = {0};

// W E A K  F U N C T I O N S
//
// @brief Write character to stdout ring buffer.
//
// @param ch character to write to stdout
// @return character
//
int __io_putchar(int ch)
{
    char data = (char)ch;

    // Critical section begin.
    bspInterruptEnable(false);

    // Write to ring buffer.
    ringWrite(bsp()->hStdout, &data, sizeof(data));

    // Critical section end.
    bspInterruptEnable(true);

    return ch;
}

//
// @brief Process receive character interrupt from UART.
//
// @param pUart pointer to UART instance
// @return none
//
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *pUart)
{
    // Validate argument[s].
    ASSERT(NULL != pUart);

    // Write character to ring.
    ringWrite(bsp()->hStdin, &bsp()->stdRx, sizeof(bsp()->stdRx));

    // Receive character in interrupt mode.
    HAL_UART_Receive_IT(pUart, (uint8_t*)&bsp()->stdRx, sizeof(bsp()->stdRx));
}

// P U B L I C  F U N C T I O N S
//
// @brief Get the BSP.
//
// @return pointer to BSP structure
//
inline trBsp* bsp(void)
{
    // Validate BSP pointer.
    ASSERT(NULL != gprBsp);

    return gprBsp;
}

//
// @brief Drain console stdout buffer.
//
// @return void
//
void bspConsoleDrain(void)
{
    char ch;

    // Try to drain a character from the log.
    if (ringRead(bsp()->hStdout, &ch, sizeof(ch)) > 0)
    {
        bspConsoleWrite(ch);
    }
}

//
// @brief Determine if console stdin is empty.
//
// @return true = empty, false = not empty
//
bool bspConsoleEmpty(void)
{
    return ringEmpty(bsp()->hStdin);
}

//
// @brief Flush console stdout buffer.
//
// @return void
//
void bspConsoleFlush(void)
{
    char ch;

    // Drain character to stdout.
    while (ringRead(bsp()->hStdout, &ch, sizeof(ch)) > 0)
    {
        bspConsoleWrite(ch);
    }
}

//
// @brief Read character from console stdin.
//
// @param pCh pointer to character
// @return true = read, false = not read
//
bool bspConsoleRead(char *pCh)
{
    // Validate argument[s].
    ASSERT(NULL != pCh);

    return (ringRead(bsp()->hStdin, pCh, sizeof(*pCh)) > 0) ? true : false;
}

//
// @brief Write character to console UART.
//
// @param ch character to write to UART
// @return character
//
void bspConsoleWrite(char ch)
{
    HAL_UART_Transmit(bsp()->pUart1, (const uint8_t*)&ch, sizeof(ch),
        BSP_UART_TIMEOUT);
}

//
// @brief Read cycle counter value.
//
// @return counter value
//
inline uint32_t bspCycleCounter(void)
{
    return DWT->CYCCNT;
}

//
// @brief Retrieve cycle counter frequency.
//
// @return frequency
//
inline uint32_t bspCycleCounterFrequency(void)
{
    return BSP_CPU_CLOCK_FREQUENCY;
}

//
// @brief Suspend execution for specified time.
//
// @param microseconds number of microseconds to delay execution
// @return none
//
inline void bspMicrosecondDelay(uint32_t microseconds)
{
    uint32_t start = bspCycleCounter();
    uint32_t wait  = (microseconds * BSP_CPU_CLOCK_FREQUENCY) / 1E+06;

    // Suspend execution for specified time.
    while ((bspCycleCounter() - start) < wait);
}

//
// @brief Suspend execution for specified time.
//
// @param milliseconds number of milliseconds to delay execution
// @return none
//
inline void bspMillisecondDelay(uint32_t milliseconds)
{
    HAL_Delay(milliseconds);
}

//
// @brief Suspend execution for specified time.
//
// @note Minimum delay and resolution is dictated by the CPU clock frequency.
//
// @param nanoseconds number of nanoseconds to delay execution
// @return none
//
inline void bspNanosecondDelay(uint32_t nanoseconds)
{
    uint32_t start = bspCycleCounter();
    uint32_t wait  = (nanoseconds * BSP_CPU_CLOCK_FREQUENCY) / 1E+09;

    // Suspend execution for specified time.
    while ((bspCycleCounter() - start) < wait);
}

//
// @brief Read from I2C bus.
//
// @param address 7-bit address
// @param pBuffer pointer to data buffer
// @param length size of data buffer in bytes
// @return true = success, false = timeout
//
bool bspI2cRead(uint8_t address, uint8_t *pBuffer, uint16_t length)
{
    // Validate argument[s].
    ASSERT(NULL != pBuffer);
    ASSERT(length > 0);

    // Send data.
    if (HAL_OK != HAL_I2C_Master_Receive(bsp()->pI2c1, address,
        pBuffer, length, BSP_I2C_TIMEOUT))
    {
        return false;
    }

    return true;
}

//
// @brief Read from I2C bus register.
//
// @param address 7-bit address
// @param reg register address
// @param size register address size in bytes
// @param pBuffer pointer to data buffer
// @param length size of data buffer in bytes
// @return true = success, false = timeout
//
bool bspI2cRegRead(uint8_t address, uint16_t reg, uint16_t size,
                   uint8_t *pBuffer, uint16_t length)
{
    // Validate argument[s].
    ASSERT(NULL != pBuffer);
    ASSERT(length > 0);

    // Read register data.
    if (HAL_OK != HAL_I2C_Mem_Read(bsp()->pI2c1, (address << 1) | 0x1,
        reg, size, pBuffer, length, BSP_I2C_TIMEOUT))
    {
        return false;
    }

    return true;
}

//
// @brief Write to I2C bus register.
//
// @param address 7-bit address
// @param reg register address
// @param size register address size in bytes
// @param pBuffer pointer to data buffer
// @param length size of data buffer in bytes
// @return true = success, false = timeout
//
bool bspI2cRegWrite(uint8_t address, uint16_t reg, uint16_t size,
                   uint8_t *pBuffer, uint16_t length)

{
    // Validate argument[s].
    ASSERT(NULL != pBuffer);
    ASSERT(length > 0);

    // Write register data.
    if (HAL_OK != HAL_I2C_Mem_Write(bsp()->pI2c1, address << 1,
        reg, size, pBuffer, length, BSP_I2C_TIMEOUT))
    {
        return false;
    }

    return true;
}

//
// @brief Write to I2C bus.
//
// @param address 7-bit address
// @param pBuffer pointer to data buffer
// @param length size of data buffer in bytes
// @return true = success, false = timeout
//
bool bspI2cWrite(uint8_t address, uint8_t *pBuffer, uint16_t length)
{
    // Validate argument[s].
    ASSERT(NULL != pBuffer);
    ASSERT(length > 0);

    // Send data.
    if (HAL_OK != HAL_I2C_Master_Transmit(bsp()->pI2c1, address,
        pBuffer, length, BSP_I2C_TIMEOUT))
    {
        return false;
    }

    return true;
}

//
// @brief Initialize the BSP.
//
// @param prBsp pointer to BSP structure
// @return true = success, false = failure
//
bool bspInit(trBsp *prBsp)
{
	// Validate argument[s].
	ASSERT(NULL != prBsp);

	// Set local BSP structure pointer.
	gprBsp = prBsp;

	// Disable I/O buffering.
	setvbuf(stdout, NULL, _IONBF, 0);

	// Enable trace and cycle counter.
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	DWT->CYCCNT       = 0;
	DWT->CTRL        |= DWT_CTRL_CYCCNTENA_Msk;

    // Create ring buffers.
    bsp()->hStdin   = ringInit(gStdinBuffer, sizeof(gStdinBuffer));
    bsp()->hStdout  = ringInit(gStdoutBuffer, sizeof(gStdoutBuffer));

	// Receive character in interrupt mode.
	HAL_UART_Receive_IT(bsp()->pUart1, (uint8_t*)&bsp()->stdRx,
        sizeof(bsp()->stdRx));

	return true;
}

//
// @brief Set enable state of interrupt controller.
//
// @param state true = enable, false = disable
// @return none
//
void bspInterruptEnable(bool state)
{
	// Evaluate state.
	if (state)
	{
		__enable_irq();
	}
	else
	{
		__disable_irq();
	}
}

//
// @brief Print formatted string.
//
// @param pFormat pointer to null terminated format string
// @param ... variable arguments
// @return number of characters printed
//
int bspPrintf(const char *pFormat, ...)
{
    int         length;
    va_list     args;
    static char buffer[BSP_PRINTF_BUFFER_SIZE];

    // Print formatted string to buffer.
    //lint -save -e530
    va_start(args, pFormat);
    //lint -restore
    length = vsnprintf(buffer, sizeof(buffer), pFormat, args);
    va_end(args);

    // Critical section begin.
    bspInterruptEnable(false);

    // Write to ring buffer.
    ringWrite(bsp()->hStdout, buffer, strlen(buffer));

    // Critical section end.
    bspInterruptEnable(true);

    return length;
}

//
// @brief Reset the board.
//
// @return none
//
void bspReset(void)
{
    // Reset MCU.
    HAL_NVIC_SystemReset();
}

//
// @brief Handle tick.
//
// @return void
//
void bspTick(void)
{
	// Validate BSP structure.
	if (NULL != bsp())
	{
		// Validate callback.
		if (NULL != bsp()->pTickHandler)
		{
			// Invoke callback.
		    bsp()->pTickHandler(bsp()->pTickContext);
		}
	}
}

//
// @brief Install tick handler.
//
// @param pCallback pointer to callback function
// @param pContext optional pointer to callback context
// @return character
//
void bspTickHandler(tpBspTickHandler pHandler, void *pContext)
{
	// Validate argument[s].
	ASSERT(NULL != pHandler);

	// Disable interrupts.
	bspInterruptEnable(false);

	// Assign callback.
	bsp()->pTickHandler = pHandler;
	bsp()->pTickContext = pContext;

	// Enable interrupts.
	bspInterruptEnable(true);
}

//
// @brief Set illumination state of heartbeat LED.
//
// @param state true = on, false = off
// @return character
//
void bspHeartbeatLed(bool state)
{
    HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, (state) ?
        GPIO_PIN_SET : GPIO_PIN_RESET);
}
