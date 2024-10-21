///////////////////////////////////////////////////////////////////////////////
//
// @file shell.c
// @brief This file contains the shell implementation.
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

// T Y P E D E F S
#ifndef uint
typedef unsigned int        uint;
#endif

// S T R U C T U R E S
typedef struct
{
	bool	 allocated;
    char     name[SHELL_CMD_NAME_LENGTH];
    char     arguments[SHELL_CMD_ARG_LENGTH];
    char     description[SHELL_CMD_DESCRIPTION_LENGTH];
    char     detail[SHELL_CMD_DETAIL_LENGTH];
    uint32_t minArgs;
    tpCmd    pCmd;
} trShellCmd;

typedef struct _trLine
{
	bool     prompt;
    uint16_t position;
	char 	 buffer[SHELL_LINE_LENGTH_MAX];
} trLine;

// P R I V A T E  D A T A
//
// Shell enable.
//
static bool gShellEnable = true;

//
// Command line.
//
static trLine grLine;
static trLine grLineHistory[SHELL_LINE_HISTORY_ENTRIES];

//
// Shell commands.
//
static trShellCmd grCmd[SHELL_CMD_ENTRIES] = {0};

// P R I V A T E   F U N C T I O N S
//
// @brief Command parser.
//
// @param pString pointer to null terminated command line
// @return enumerated error code
//
static teError command(char *pString)
{
    uint32_t  index;
    int       argc;
    char 	 *argv[SHELL_CMD_ARGS_MAX];

    // Tokenize command line.
    argv[argc = 0] = strtok(pString, " ");
    while ((argc < (int)DIM(argv)) && (NULL != argv[argc]))
    {
        //lint -save -e661
        argv[++argc] = strtok(NULL, " ");
        //lint -restore
    }

    // Check for no command.
    if (0 == argc)
    {
        return eErrorNone;
    }

    // Iterate over commands.
    for (index = 0; index < DIM(grCmd); index++)
    {
        // Check for matching string.
        if (0 == strcmp(argv[0], grCmd[index].name))
        {
            // Check for minimum arguments.
            if (argc < (int)grCmd[index].minArgs)
            {
                return eErrorArgument;
            }

            // Execute command.
            return grCmd[index].pCmd(argc, argv);
        }
    }

    return eErrorUnsupported;
}

//
// @brief Clear the terminal screen.
//
// @param argc number of arguments
// @param argv array of pointers to arguments
// @return enumerated error code
//
static teError clear(int argc, char *argv[])
{
    bspPrintf(VT100_CLEAR_SCREEN);

    return eErrorNone;
}

//
// @brief Print build date and time.
//
// @param argc number of arguments
// @param argv array of pointers to arguments
// @return enumerated error code
//
static teError build(int argc, char *argv[])
{
    bspPrintf("\r\n%s %s", __DATE__, __TIME__);

    return eErrorNone;
}

//
// @brief Comment, do nothing.
//
// @param argc number of arguments
// @param argv array of pointers to arguments
// @return enumerated error code
//
static teError comment(int argc, char *argv[])
{
    return eErrorNone;
}

//
// @brief Delay execution for a specified time.
//
// @param argc number of arguments
// @param argv array of pointers to arguments
// @return enumerated error code
//
static teError delay(int argc, char *argv[])
{
    uint32_t milliseconds;

    // Parse command line.
    milliseconds = strtol(argv[1], NULL, 0);

    // Apply correction.
    milliseconds = (milliseconds > 0) ? milliseconds - 1 : 0;

	// Wait a bit.
	bspMillisecondDelay(milliseconds);

    return eErrorNone;

}

//
// @brief Print error history.
//
// @param argc number of arguments
// @param argv array of pointers to arguments
// @return enumerated error code
//
static teError error(int argc, char *argv[])
{
	static trLogError rError;

	// Dump error history.
    while (logErrorHistory(&rError))
	{
		bspPrintf("\r\n%d,%s,%lu", rError.eError, rError.file, rError.line);
	}

	return eErrorNone;
}

//
// @brief Print command usage information.
//
// @param argc number of arguments
// @param argv array of pointers to arguments
// @return enumerated error code
//
static teError help(int argc, char *argv[])
{
    uint32_t index;

    // Check for specific command.
    if (2 == argc)
    {
        // Iterate over commands.
        for (index = 0; index < DIM(grCmd); index++)
        {
            // Check for matching string.
            if (0 == strcmp(argv[1], grCmd[index].name))
            {
                // Print command usage.
                bspPrintf("\r\n%s %s", grCmd[index].name,
                    grCmd[index].arguments);
                bspPrintf("\r\n%s", grCmd[index].detail);

                return eErrorNone;
            }
        }

        return eErrorArgument;
    }

    // Iterate over commands.
    for (index = 0; index < DIM(grCmd); index++)
    {
        // Print command usage.
    	if (grCmd[index].allocated)
    	{
			bspPrintf("\r\n%8s %-40s %-28s", grCmd[index].name,
				grCmd[index].arguments, grCmd[index].description);
    	}
    }

    return eErrorNone;
}

//
// @brief Read from i2C bus.
//
// @param argc argument count
// @param argv pointer to array of pointers to null terminated argument strings
// @return enumerated error code
//
static teError i2cRead(int argc, char *argv[])
{
    uint8_t      buffer[BSP_I2C_BUFFER_MAX];
    uint8_t      address;
    uint16_t     index;
    uint16_t     length;

    // Parse arguments.
    address  = (uint8_t)strtol(argv[1], NULL, 0);
    length   = (uint16_t)strtol(argv[2], NULL, 0);

    // Santize length.
    length   = (length > sizeof(buffer)) ? sizeof(buffer) : length;

    // Read bus.
    if (!bspI2cRead(address, buffer, length))
    {
        return eErrorTimeout;
    }

    // Print data.
    bspPrintf("\r\n");
    for (index = 0; index < length; index++)
    {
        bspPrintf("0x%02x ", buffer[index]);
    }

    return eErrorNone;
}

//
// @brief Write to i2C bus.
//
// @param argc argument count
// @param argv pointer to array of pointers to null terminated argument strings
// @return enumerated error code
//
static teError i2cWrite(int argc, char *argv[])
{
    uint8_t      buffer[BSP_I2C_BUFFER_MAX];
    uint8_t      address;
    uint16_t     length;

    // Parse arguments.
    address = (uint8_t)strtol(argv[1], NULL, 0);
    for (length = 0; ((length + 2) < argc) &&
        (length < sizeof(buffer)); length++)
    {
        buffer[length] = (uint8_t)strtol(argv[length + 2], NULL, 0);
    }

    // Write channel.
    if (!bspI2cWrite(address, buffer, length))
    {
        return eErrorTimeout;
    }

    return eErrorNone;
}

//
// @brief Monitor command.
//
// @param argc number of arguments
// @param argv array of pointers to arguments
// @return enumerated error code
//
static teError monitor(int argc, char *argv[])
{
    int                index;
    uint32_t           milliseconds;
    char    	   	   buffer[SHELL_LINE_LENGTH_MAX];
    static char const *pSpace = " ";
    teError			   eError;

    // Parse command line.
    milliseconds = strtol(argv[1], NULL, 0);

    // Apply correction.
    milliseconds = (milliseconds > 0) ? milliseconds - 1 : 0;

    // Wait for user abort.
    while (bspConsoleEmpty())
    {
        // Clear screen and place cursor in home position.
        bspPrintf(VT100_CLEAR_SCREEN);
        bspPrintf(VT100_CURSOR_HOME);

        // Build command buffer.
        memset(buffer, 0, sizeof(buffer));
        for (index = 2; index < argc; index++)
        {
            strncat(buffer, argv[index], sizeof(buffer)-1);
            strncat(buffer, pSpace, sizeof(buffer)-1);
        }

        // Execute command.
        if (eErrorNone != (eError = command(buffer)))
        {
            return eError;
        }

        // Wait for specified time.
        bspMillisecondDelay(milliseconds);

        // Service timers.
        timerService();

        // Try to drain stdout.
        bspConsoleFlush();
    }

    return eErrorNone;
}

//
// @brief Read from memory.
//
// @param argc number of arguments
// @param argv array of pointers to arguments
// @return enumerated error code
//
static teError memRead(int argc, char *argv[])
{
    uint 	  index;
    uint 	  address;
    uint 	  offset;
    uint8_t   buffer[SHELL_ASCII_BUFFER_LENGTH];
    char 	  width;
    uint 	  length;
	uint8_t  *pByte;
	uint16_t *pHalf;
	uint32_t *pWord;

    // Set defaults.
    width  = 'w';
    length = 1;

    // Parse arguments.
    address = strtol(argv[1], NULL, 0);
    if (argc > 2)
    {
    	length = strtol(argv[2], NULL, 0);
        if (0 == length)
        {
            return eErrorArgument;
        }
    }
    if (argc > 3)
    {
        width = *argv[3];
    }

    // Assign address types.
    pByte = (uint8_t*)address;
    pHalf = (uint16_t*)address;
    pWord = (uint32_t*)address;

    // Read and display memory contents.
    memset(buffer, 0, sizeof(buffer));
    for (index = 0; bspConsoleEmpty() && (index < length); index++)
    {
        // Process width.
        switch (width)
        {
            case 'b':
                if (0 == index%sizeof(buffer))
                {
                    if (index > 0)
                    {
                        bspPrintf(" ");
                        for (offset = 0; offset < sizeof(buffer); offset++)
                        {
                            bspPrintf("%c",
                                (isprint(buffer[offset])) ?
                                buffer[offset] : '.');
                        }
                    }
                    bspPrintf("\r\n%08lx: ", (uint32_t)pByte);
                }
                buffer[index%sizeof(buffer)] = (char)*pByte++;
                bspPrintf("%02x ", buffer[index%sizeof(buffer)] & 0xff);
                break;

            case 'h':
                if (0 != address%(sizeof(uint16_t)))
                {
                    return eErrorArgument;
                }
                if (0 == index%(sizeof(buffer)/sizeof(uint16_t)))
                {
                    bspPrintf("\r\n%08lx: ", (uint32_t)pHalf);
                }
                bspPrintf("%04x ", *pHalf++);
                break;

            case 'w':
                    if (0 != address%(sizeof(uint32_t)))
                    {
                        return eErrorArgument;
                    }
                    if (0 == index%(sizeof(buffer)/sizeof(uint32_t)))
                    {
                        bspPrintf("\r\n%08lx: ", (uint32_t)pWord);
                    }
                    bspPrintf("%08lx ", *pWord++);
                    break;

            default:
                    return eErrorArgument;
        }
    }

    // Print remaining character line.
    if ('b' == width)
    {
        bspPrintf(" ");
        length = length%sizeof(buffer);
        length = (0 == length) ? sizeof(buffer) : length;
        for (index = 0; index < (sizeof(buffer)-length); index++)
        {
            bspPrintf("   ");
        }
        for (offset = 0; offset < length; offset++)
        {
            bspPrintf("%c", (isprint(buffer[offset])) ?
                buffer[offset] : '.');
        }
    }

    return eErrorNone;
}

//
// @brief Write to memory.
//
// @param argc number of arguments
// @param argv array of pointers to arguments
// @return enumerated error code
//
static teError memWrite(int argc, char *argv[])
{
    uint  	  index;
    uint  	  address;
    uint  	  value;
    char  	  width;
    uint  	  length;
	uint8_t  *pByte;
	uint16_t *pHalf;
	uint32_t *pWord;


    // Set defaults.
    width  = 'w';
    length = 1;

    // Parse arguments.
    address = strtol(argv[1], NULL, 0);
    value   = strtol(argv[2], NULL, 0);
    if (argc > 3)
    {
        length = strtol(argv[3], NULL, 0);
    }
    if (argc > 4)
    {
        width = *argv[4];
    }

    // Assign address types.
    pByte = (uint8_t*)address;
    pHalf = (uint16_t*)address;
    pWord = (uint32_t*)address;

    // Write to memory.
    for (index = 0; index < length; index++)
    {
        // Process width.
        switch (width)
        {
            case 'b':
            	*pByte++ = value;
                break;

            case 'h':
            	*pHalf++ = value;
                break;

            case 'w':
            	*pWord++ = value;
                break;

            default:
                return eErrorArgument;
        }
    }

    return eErrorNone;
}

//
// @brief Execution command.
//
// @param argc number of arguments
// @param argv array of pointers to arguments
// @return enumerated error code
//
static teError xtime(int argc, char *argv[])
{
    int                index;
    uint32_t           start;
    uint32_t		   delta;
    char               buffer[SHELL_LINE_LENGTH_MAX];
    static char const *pSpace = " ";
    teError			   eError;

    // Build command buffer.
    memset(buffer, 0, sizeof(buffer));
    for (index = 1; index < argc; index++)
    {
        strncat(buffer, argv[index], sizeof(buffer)-1);
        strncat(buffer, pSpace, sizeof(buffer)-1);
    }

    // Sample start time.
    start = bspCycleCounter();

    // Execute command.
    eError = command(buffer);

    // Compute duration.
    delta = bspCycleCounter() - start;

    // Display execution time.
    bspPrintf("\r\n%1.6E", (double)delta / (double)bspCycleCounterFrequency());

    return eError;
}

//
// @brief Performance command.
//
// @param argc number of arguments
// @param argv array of pointers to arguments
// @return enumerated error code
//
static teError perf(int argc, char *argv[])
{
    // Dump all performance stats.
    perfPrint(NULL);

    return eErrorNone;
}

//
// @brief Repeat command.
//
// @param argc number of arguments
// @param argv array of pointers to arguments
// @return enumerated error code
//
static teError repeat(int argc, char *argv[])
{
    int                index;
    int                count;
    uint32_t           milliseconds;
    char               buffer[SHELL_LINE_LENGTH_MAX];
    static char const *pSpace = " ";
    teError			   eError;

    // Parse command line.
    count = strtol(argv[1], NULL, 0);
    if (0 == count)
    {
        return eErrorArgument;
    }
    milliseconds = strtol(argv[2], NULL, 0);

    // Apply correction.
    milliseconds = (milliseconds > 0) ? milliseconds - 1 : 0;

    // Wait for user abort or terminal count.
    while (bspConsoleEmpty() && (count-- > 0))
    {
        // Build command buffer.
        memset(buffer, 0, sizeof(buffer));
        for (index = 3; index < argc; index++)
        {
            strncat(buffer, argv[index], sizeof(buffer)-1);
            strncat(buffer, pSpace, sizeof(buffer)-1);
        }

        // Execute command.
        if (eErrorNone != (eError = command(buffer)))
        {
            return eError;
        }

        // Wait for specified time.
        bspMillisecondDelay(milliseconds);

        // Service timers.
        timerService();

        // Flush stdout.
        bspConsoleFlush();
    }

    return eErrorNone;
}

//
// @brief Reboot command.
//
// @param argc number of arguments
// @param argv array of pointers to arguments
// @return enumerated error code
//
static teError reboot(int argc, char *argv[])
{
	bspReset();

    return eErrorNone;
}

//
// @brief Print uptime.
//
// @param argc number of arguments
// @param argv array of pointers to arguments
// @return enumerated error code
//
static teError uptime(int argc, char *argv[])
{
    trTimerTimestamp rTimestamp;

    // Get timestamp.
    timerTimestamp(&rTimestamp);

    // Print update.
    bspPrintf("\r\n%d.%02u:%02u:%02u.%03u", rTimestamp.days, rTimestamp.hours,
        rTimestamp.minutes, rTimestamp.seconds, rTimestamp.milliseconds);

    return eErrorNone;
}

// P U B L I C  F U N C T I O N S
//
// @brief Initialize shell.
//
// @return enumerated error code
//
teError shellInit(void)
{
	teError eError;

	// Initialize line and history buffers.
	memset(&grLine, 0, sizeof(grLine));
    memset(&grLineHistory, 0, sizeof(grLineHistory));

    // Clear commands.
    memset(grCmd, 0, sizeof(grCmd));

    // Add built-in commands.
    if (eErrorNone != (eError = shellAdd("build",
        "build date/time", "", "", 1, build)))
    {
        LOGERROR(eError);
    }
    if (eErrorNone != (eError = shellAdd("#",
        "comment", "", "", 1, comment)))
    {
        LOGERROR(eError);
    }
    if (eErrorNone != (eError = shellAdd("clear",
		"clear the terminal screen", "", "", 1, clear)))
    {
    	LOGERROR(eError);
    }
    if (eErrorNone != (eError = shellAdd("delay",
        "delay for a period of time", "<milliseconds>",
        "milliseconds=<0..n>",
        2, delay)))
    {
        LOGERROR(eError);
    }
    if (eErrorNone != (eError = shellAdd("error",
		"print error history", "", "", 1, error)))
    {
    	LOGERROR(eError);
    }
    if (eErrorNone != (eError = shellAdd("help",
		"print help information", "[cmd]",
		"cmd=<any supported command>", 1, help)))
    {
    	LOGERROR(eError);
    }
    if (eErrorNone != (eError = shellAdd("ir",
        "i2c read", "<address> <length>",
        "address=<7-bit>, length=<1-n>", 2, i2cRead)))
    {
        LOGERROR(eError);
    }
    if (eErrorNone != (eError = shellAdd("iw",
        "i2c write", "<address> <data>",
        "address=<7-bit>, data=<...>", 2, i2cWrite)))
    {
        LOGERROR(eError);
    }
    if (eErrorNone != (eError = shellAdd("mon",
		"monitor command", "<milliseconds> <cmd>",
		"milliseconds=<0..n>, cmd=<any supported command>",
		3, monitor)))
    {
    	LOGERROR(eError);
    }
    if (eErrorNone != (eError = shellAdd("mr",
		"memory read", "<address> [length] [b|h|w]",
		"address=<hex>, length=<dec>, b=byte, h=halfword, w=word",
		2, memRead)))
    {
    	LOGERROR(eError);
    }
    if (eErrorNone != (eError = shellAdd("mw",
		"memory write", "<address> <value> [length] [b|h|w]",
		"address=<hex>, value=<hex>, length=<dec>, b=byte, h=half, w=word",
		3, memWrite)))
    {
    	LOGERROR(eError);
    }
    if (eErrorNone != (eError = shellAdd("perf",
        "performance statistics", "", "",
        1, perf)))
    {
        LOGERROR(eError);
    }
    if (eErrorNone != (eError = shellAdd("reboot",
        "reboot firmware", "", "",
        1, reboot)))
    {
        LOGERROR(eError);
    }
    if (eErrorNone != (eError = shellAdd("rep",
		"repeat command", "<count> <milliseconds> <cmd>",
		"count=<1..n>, milliseconds=<0..n>, cmd=<any supported command>",
		4, repeat)))
    {
    	LOGERROR(eError);
    }
    if (eErrorNone != (eError = shellAdd("xtime",
		"measure execution time", "<cmd>",
		"cmd=<any supported command>",
		2, xtime)))
    {
    	LOGERROR(eError);
    }
    if (eErrorNone != (eError = shellAdd("up",
		"print time since boot", "", "",
		1, uptime)))
    {
    	LOGERROR(eError);
    }

	return eErrorNone;
}

//
// @brief Service shell activity.
//
// @return enumerated error code
//
teError shellService(void)
{
	char        ch;
	static uint history = 0;
	teError     eError;

	// Check state.
	if (!gShellEnable)
	{
		return eErrorNone;
	}

	// Check prompt.
	if (!grLine.prompt)
	{
		// Print prompt.
		bspPrintf("\r\n\n%s ", SHELL_PROMPT);

		// Set state.
		grLine.prompt = true;
	}

	// Try to read character.
	if (bspConsoleRead(&ch))
	{
		// Process character.
		switch (ch)
		{
			case '\b':

				// Check character count.
				if (grLine.position > 0)
				{
					// Echo backspace sequence.
					bspPrintf("\b \b");

					// Delete character.
					grLine.buffer[--grLine.position] = 0;
				}
				break;

			case '\r':
			case '\n':

				// Print newline.
		        putchar(ch);

		        // Preserve command in circular history buffer.
		        memcpy(&grLineHistory[history], &grLine, sizeof(grLineHistory[history]));
		        history = (history + 1)%DIM(grLineHistory);

		        // Process command.
		        if (eErrorNone != (eError = command(grLine.buffer)))
		        {
		            bspPrintf("\r\nmalformed or failed command (%s)",
						errorString(eError));
		        }

				// Clear line buffer.
				memset(&grLine, 0, sizeof(grLine));

				return eErrorNone;

			case '\e':

		        // Read character.
		        bspConsoleRead(&ch);

		        // Process character.
		        switch (ch)
		        {
		            case '[':

		                // Read character.
		                bspConsoleRead(&ch);

		                // Process character.
		                switch (ch)
		                {
		                    // Up arrow.
		                    case 'A':
		                        history = (history - 1 + DIM(grLineHistory))%DIM(grLineHistory);
		                        if (strlen(grLineHistory[history].buffer) > 0)
		                        {
                                    memcpy(&grLine, &grLineHistory[history], sizeof(grLine));
                                    grLine.prompt = true;
                                    bspPrintf(VT100_CLEAR_LINE);
                                    bspPrintf("\r%s %s", SHELL_PROMPT, grLine.buffer);
		                        }
		                        break;

                            // Down arrow.
		                    case 'B':
		                        history = (history + 1)%DIM(grLineHistory);
                                if (strlen(grLineHistory[history].buffer) > 0)
                                {
                                    memcpy(&grLine, &grLineHistory[history], sizeof(grLine));
                                    grLine.prompt = true;
                                    bspPrintf(VT100_CLEAR_LINE);
                                    bspPrintf("\r%s %s", SHELL_PROMPT, grLine.buffer);
                                }
                                break;

                            // Right arrow.
                            case 'C':
                                break;

                            // Left arrow.
                            case 'D':
                                break;

		                    default:
		                        break;
		                }

		            default:
		                break;

		        }
			    break;

			default:

				// Check for printable character and acceptable length
				if (isprint(ch) && (grLine.position < sizeof(grLine.buffer)))
				{
					// Echo character.
					putchar(ch);

					// Add character to buffer.
					grLine.buffer[grLine.position++] = (char)ch;
				}
				break;
		}
	}

	return eErrorNone;
}

//
// @brief Add shell command.
//
// @param pName pointer to null terminated command name string
// @param pDescription pointer to null terminated command description string
// @param pArguments pointer to null terminated command arguments string
// @param pDetail pointer to null terminated command arguments detail string
// @param minArgs minimum number of arguments
// @param pCmd pointer to command function
// @return enumerated error code
//
teError shellAdd(const char *pName, const char *pDescription,
			     const char *pArguments, const char *pDetail,
				 uint8_t minArgs, tpCmd pCmd)
{
	int index;

	// Validate argument[s].
	ASSERT(NULL != pName);
	ASSERT(NULL != pArguments);
	ASSERT(NULL != pDescription);
	ASSERT(NULL != pDetail);
	ASSERT(minArgs > 0);
	ASSERT(NULL != pCmd);

	// Iterate over command array.
	for (index = 0; index < DIM(grCmd); index++)
	{
		// Check for non-allocated command.
		if (!grCmd[index].allocated)
		{
			// Assign command.
			strncpy(grCmd[index].name, pName, sizeof(grCmd[index].name)-1);
			strncpy(grCmd[index].arguments, pArguments, sizeof(grCmd[index].arguments)-1);
			strncpy(grCmd[index].description, pDescription, sizeof(grCmd[index].description)-1);
			strncpy(grCmd[index].detail, pDetail, sizeof(grCmd[index].detail)-1);
			grCmd[index].minArgs = minArgs;
			grCmd[index].pCmd    = pCmd;

			// Mark as allocated.
			grCmd[index].allocated = true;

			return eErrorNone;
		}
	}

	return eErrorOutOfResources;
}

