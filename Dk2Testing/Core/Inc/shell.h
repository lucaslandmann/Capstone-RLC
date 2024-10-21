///////////////////////////////////////////////////////////////////////////////
//
// @file shell.h
// @brief This file contains the shell interface definition.
//
//  COPYRIGHT (c) 2022 Summit Scientific, Inc. All rights reserved.
//  NOTICE:  Contains confidential and proprietary information of
//           Summit Scientific, Inc. and is protected under the copyright
//           laws as an unpublished work.
//
// @author R. Van Rooyen
//
///////////////////////////////////////////////////////////////////////////////

#ifndef SHELL_H
#define SHELL_H

// D E F I N E S
#define SHELL_CMD_ENTRIES				40
#define SHELL_CMD_NAME_LENGTH			8
#define SHELL_CMD_ARG_LENGTH			40
#define SHELL_CMD_DESCRIPTION_LENGTH	28
#define SHELL_CMD_DETAIL_LENGTH			80
#define SHELL_LINE_LENGTH_MAX			160
#define SHELL_LINE_HISTORY_ENTRIES      8
#define SHELL_ASCII_BUFFER_LENGTH		16
#define SHELL_CMD_ARGS_MAX				32
#define SHELL_PROMPT					"$"


// T Y P E D E F S
typedef teError (*tpCmd)(int argc, char *argv[]);

// P U B L I C  F U N C T I O N S
#ifdef __cplusplus
extern "C" {
#endif

teError shellInit(void);
teError shellAdd(const char *pName, const char *pArguments,
			     const char *pDescription, const char *pDetail,
				 uint8_t minArgs, tpCmd pCmd);
teError shellService(void);

#ifdef __cplusplus
}
#endif

#endif // SHELL_H
