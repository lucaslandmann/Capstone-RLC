///////////////////////////////////////////////////////////////////////////////
//
// @file log.h
// @brief This file contains the log interface definition.
//
//  COPYRIGHT (c) 2022 Summit Scientific, Inc. All rights reserved.
//  NOTICE:  Contains confidential and proprietary information of
//           Summit Scientific, Inc. and is protected under the copyright
//           laws as an unpublished work.
//
// @author R. Van Rooyen
//
///////////////////////////////////////////////////////////////////////////////

#ifndef LOG_H
#define LOG_H

// D E F I N E S
#define LOG_LINE_LENGTH_MAX     	80
#define LOG_ERROR_HISTORY_DEPTH		8
#define LOG_FILE_LENGTH_MAX         32

// M A C R O S
#ifdef LOG_ENABLE
#define ASSERT(i)					logAssert(i, __FILE__, __LINE__, \
										__func__, #i)
#define LOG(l, f, ...)				logEntry(l, __FILE__, __LINE__, \
										__func__, f, ##__VA_ARGS__)
#define LOGDATA(l, t, b, s)     	logDataEntry(l, __FILE__, __LINE__, \
										__func__, t, (void*)b, s)
#define LOGERROR(e)					logError(e, __FILE__, __LINE__, \
										__func__)
#define LOGFATAL(e)					logFatal(e, __FILE__, __LINE__, \
										__func__)
#else
#define ASSERT(i)					(void*)(0)
#define LOG(l, f, ...)				(void*)(0)
#define LOGDATA(l, t, b, s)     	(void*)(0)
#define LOGERROR(e)					(void*)(0)
#define LOGFATAL(e)					(void*)(0)
#endif

// E N U M S
typedef enum
{
    eLogLevelNone   = 0,
    eLogLevelFatal  = 1,
    eLogLevelAssert = 2,
    eLogLevelError  = 3,
    eLogLevelWarn   = 4,
    eLogLevelInfo   = 5,
    eLogLevelDebug  = 6,
    eLogLevelMax    = 7
} teLogLevel;

// S T R U C T U R E S
typedef struct
{
    teError  eError;
    char     file[LOG_FILE_LENGTH_MAX];
    uint32_t line;
} trLogError;

// P U B L I C  F U N C T I O N S
#ifdef __cplusplus
extern "C" {
#endif

teError 	 logInit(void);
void         logDrain(void);
void  	 	 logAssert(bool bIgnore, const char *pFilename,
                       const uint16_t line, const char *pFunction,
                       const char *pDescription);
void 	 	 logLevelSet(teLogLevel eLogLevel);
teLogLevel	 logLevelGet(void);
void 	 	 logEntry(teLogLevel eLogLevel, const char *pFilename,
                      const uint16_t line, const char *pFunction,
                      const char *pFormat, ...);
void 	 	 logDataEntry(teLogLevel eLogLevel, const char *pFilename,
                      const uint16_t line, const char *pFunction,
                      const char *pTitle, void *pBuffer,
                      uint32_t length);
teError		 logError(teError eError, const char *pFilename,
                      const uint16_t line, const char *pFunction);
void     	 logFatal(teError eError, const char *pFilename,
                      const uint16_t line, const char *pFunction);
bool		 logErrorHistory(trLogError *pError);

#ifdef __cplusplus
}
#endif

#endif // LOG_H
