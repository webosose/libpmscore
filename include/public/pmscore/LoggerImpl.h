// @@@LICENSE
//
//      Copyright (c) 2017 LG Electronics, Inc.
//
// Confidential computer software. Valid license from LG required for
// possession, use or copying. Consistent with FAR 12.211 and 12.212,
// Commercial Computer Software, Computer Software Documentation, and
// Technical Data for Commercial Items are licensed to the U.S. Government
// under vendor's standard commercial license.
//
// LICENSE@@@

#ifndef PMS_LOGGERIMPL_H
#define PMS_LOGGERIMPL_H


#include <stdarg.h>

#include <PmLogLib.h>

#include "Uncopyable.h"


#ifdef __GNUC__
#define PMS_LIKE_PRINTF __attribute__((__format__(__printf__, 2, 3)))
#define PMS_VLOGTEXT_FMT_CHK __attribute__((__format__(__printf__, 3, 0)))
#else
#define PMS_LIKE_PRINTF
#define PMS_VLOGTEXT_FMT_CHK
#endif

#define PMS_LOGGER_MSG_ID "pms"

/**
*******************************************************************************
* @brief Debug logger class.
*******************************************************************************
*/
class Logger : private Uncopyable
{
public:
    enum LogLevel_t  {
        kLogLevelEmergency,
        kLogLevelAlert,
        kLogLevelCritical,
        kLogLevelError,
        kLogLevelWarning,
        kLogLevelNotice,
        kLogLevelInfo,
        kLogLevelDebug,
        kLogLevelLast
    };

    /**
    *******************************************************************************
    * @brief Create a logger object.
    *
    * @param pContextName String describing the logging context
    *                     (example: "PMS.IPC").
    *******************************************************************************
    */
    Logger(const char* pContextName)
    {
        PmLogGetContext(pContextName, &context_);
    }

    /**
    *******************************************************************************
    * @brief Check if the specified logger level is enabled.
    *
    * @param level The logging level to check.
    *
    *******************************************************************************
    */
    bool IsEnabled(LogLevel_t level) const
    {
        return PmLogIsEnabled(context_, aPmLogLevels_[level]);
    }

    /**
    *******************************************************************************
    * @brief Log a message using logging level "Debug".
    *
    * @param pFmt printf() style format string.
    *******************************************************************************
    */
    void Debug(const char* pFmt, ...) const PMS_LIKE_PRINTF
    {
        va_list args;
        va_start(args, pFmt);
#ifdef PMLOGLIB_3
        PmLogDebug(context_,pFmt,args);
#else
        PmLogVPrint(context_, kPmLogLevel_Debug, pFmt, args);
#endif
        va_end(args);
    }

    /**
    *******************************************************************************
    * @brief Log binary data using logging level "Debug".
    *
    * @param pData pointer to the binary data.
    * @param len   length of data (in bytes) pointed to by pData
    *******************************************************************************
    */
    void DebugBin(const void* pData, size_t len) const
    {
        len = len > kLoggerMaxBinaryMsgSize ? kLoggerMaxBinaryMsgSize : len;
        PmLogDumpData(context_, kPmLogLevel_Debug, pData, len, kPmLogDumpFormatDefault);
    }

    /**
    *******************************************************************************
    * @brief Log a message using logging level "Info".
    *
    * @param pFmt printf() style format string.
    *******************************************************************************
    */
    void Info(const char* pFmt, ...) const PMS_LIKE_PRINTF
    {
        va_list args;
        va_start(args, pFmt);
#ifdef PMLOGLIB_3
        PmLogInfo(context_,PMS_LOGGER_MSG_ID,0,pFmt,args);
#else
        PmLogVPrint(context_, kPmLogLevel_Info, pFmt, args);
#endif
        va_end(args);
    }

    /**
    *******************************************************************************
    * @brief Log binary data using logging level "Info".
    *
    * @param pData pointer to the binary data.
    * @param len   length of data (in bytes) pointed to by pData
    *******************************************************************************
    */
    void InfoBin(const void* pData, size_t len) const
    {
        len = len > kLoggerMaxBinaryMsgSize ? kLoggerMaxBinaryMsgSize : len;
        PmLogDumpData(context_, kPmLogLevel_Info, pData, len, kPmLogDumpFormatDefault);
    }

    /**
    *******************************************************************************
    * @brief Log a message using logging level "Notice".
    *
    * @param pFmt printf() style format string.
    *******************************************************************************
    */
    void Notice(const char* pFmt, ...) const PMS_LIKE_PRINTF
    {
        va_list args;
        va_start(args, pFmt);
#ifdef PMLOGLIB_3
        PmLogInfo(context_,WSG_LOGGER_MSG_ID,0,pFmt,args);
#else
        PmLogVPrint(context_, kPmLogLevel_Notice, pFmt, args);
#endif
        va_end(args);
    }

    /**
    *******************************************************************************
    * @brief Log binary data using logging level "Notice".
    *
    * @param pData pointer to the binary data.
    * @param len   length of data (in bytes) pointed to by pData
    *******************************************************************************
    */
    void NoticeBin(const void* pData, size_t len) const
    {
        len = len > kLoggerMaxBinaryMsgSize ? kLoggerMaxBinaryMsgSize : len;
#ifdef PMLOGLIB_3
        PmLogDumpData(context_, kPmLogLevel_Info, pData, len, kPmLogDumpFormatDefault);
#else
        PmLogDumpData(context_, kPmLogLevel_Notice, pData, len, kPmLogDumpFormatDefault);
#endif
    }

    /**
    *******************************************************************************
    * @brief Log a message using logging level "Warning".
    *
    * @param pFmt printf() style format string.
    *******************************************************************************
    */
    void Warning(const char* pFmt, ...) const PMS_LIKE_PRINTF
    {
        va_list args;
        va_start(args, pFmt);
#ifdef PMLOGLIB_3
        PmLogWarning(context_,WSG_LOGGER_MSG_ID,0,pFmt,args);
#else
        PmLogVPrint(context_, kPmLogLevel_Warning, pFmt, args);
#endif
        va_end(args);
    }

    /**
    *******************************************************************************
    * @brief Log binary data using logging level "Warning".
    *
    * @param pData pointer to the binary data.
    * @param len   length of data (in bytes) pointed to by pData
    *******************************************************************************
    */
    void WarningBin(const void* pData, size_t len) const
    {
        len = len > kLoggerMaxBinaryMsgSize ? kLoggerMaxBinaryMsgSize : len;
        PmLogDumpData(context_, kPmLogLevel_Warning, pData, len, kPmLogDumpFormatDefault);
    }

    /**
    *******************************************************************************
    * @brief Log a message using logging level "Error".
    *
    * @param pFmt printf() style format string.
    *******************************************************************************
    */
    void Error(const char* pFmt, ...) const PMS_LIKE_PRINTF
    {
        va_list args;
        va_start(args, pFmt);
#ifdef PMLOGLIB_3
        PmLogError(context_,WSG_LOGGER_MSG_ID,0,pFmt,args);
#else
        PmLogVPrint(context_, kPmLogLevel_Error, pFmt, args);
#endif

        va_end(args);
    }

    /**
    *******************************************************************************
    * @brief Log binary data using logging level "Error".
    *
    * @param pData pointer to the binary data.
    * @param len   length of data (in bytes) pointed to by pData
    *******************************************************************************
    */
    void ErrorBin(const void* pData, size_t len) const
    {
        len = len > kLoggerMaxBinaryMsgSize ? kLoggerMaxBinaryMsgSize : len;
        PmLogDumpData(context_, kPmLogLevel_Error, pData, len, kPmLogDumpFormatDefault);
    }

    /**
    *******************************************************************************
    * @brief Log a message using logging level "Critical".
    *
    * @param pFmt printf() style format string.
    *******************************************************************************
    */
    void Critical(const char* pFmt, ...) const PMS_LIKE_PRINTF
    {
        va_list args;
        va_start(args, pFmt);

#ifdef PMLOGLIB_3
        PmLogCritical(context_,WSG_LOGGER_MSG_ID,0,pFmt, args);
#else
        PmLogVPrint(context_, kPmLogLevel_Critical, pFmt, args);
#endif

        va_end(args);
    }

    /**
    *******************************************************************************
    * @brief Log binary data using logging level "Critical".
    *
    * @param pData pointer to the binary data.
    * @param len   length of data (in bytes) pointed to by pData
    *******************************************************************************
    */
    void CriticalBin(const void* pData, size_t len) const
    {
        len = len > kLoggerMaxBinaryMsgSize ? kLoggerMaxBinaryMsgSize : len;
        PmLogDumpData(context_, kPmLogLevel_Critical, pData, len, kPmLogDumpFormatDefault);
    }

    /**
    *******************************************************************************
    * @brief Log a message using logging level "Alert".
    *
    * @param pFmt printf() style format string.
    *******************************************************************************
    */
    void Alert(const char* pFmt, ...) const PMS_LIKE_PRINTF
    {
        va_list args;
        va_start(args, pFmt);
#ifdef PMLOGLIB_3
        PmLogCritical(context_,WSG_LOGGER_MSG_ID,0,pFmt, args);
#else
        PmLogVPrint(context_, kPmLogLevel_Alert, pFmt, args);
#endif
        va_end(args);
    }

    /**
    *******************************************************************************
    * @brief Log binary data using logging level "Alert".
    *
    * @param pData pointer to the binary data.
    * @param len   length of data (in bytes) pointed to by pData
    *******************************************************************************
    */
    void AlertBin(const void* pData, size_t len) const
    {
        len = len > kLoggerMaxBinaryMsgSize ? kLoggerMaxBinaryMsgSize : len;
#ifdef PMLOGLIB_3
        PmLogDumpData(context_, kPmLogLevel_Critical, pData, len, kPmLogDumpFormatDefault);
#else
        PmLogDumpData(context_, kPmLogLevel_Alert, pData, len, kPmLogDumpFormatDefault);
#endif
    }

    /**
    *******************************************************************************
    * @brief Log a message using logging level "Emergency".
    *
    * @param pFmt printf() style format string.
    *******************************************************************************
    */
    void Emergency(const char* pFmt, ...) const PMS_LIKE_PRINTF
    {
        va_list args;
        va_start(args, pFmt);

#ifdef PMLOGLIB_3
         PmLogCritical(context_,WSG_LOGGER_MSG_ID,0,pFmt, args);
#else
        PmLogVPrint(context_, kPmLogLevel_Emergency, pFmt, args);
#endif
        va_end(args);
    }

    /**
    *******************************************************************************
    * @brief Log binary data using logging level "Emergency".
    *
    * @param pData pointer to the binary data.
    * @param len   length of data (in bytes) pointed to by pData
    *******************************************************************************
    */
    void EmergencyBin(const void* pData, size_t len) const
    {
        len = len > kLoggerMaxBinaryMsgSize ? kLoggerMaxBinaryMsgSize : len;
#ifdef PMLOGLIB_3
        PmLogDumpData(context_, kPmLogLevel_Critical, pData, len, kPmLogDumpFormatDefault);
#else
        PmLogDumpData(context_, kPmLogLevel_Emergency, pData, len, kPmLogDumpFormatDefault);
#endif
    }

    /**
    *******************************************************************************
    * @brief var-arg style method for logging a text message
    *
    * @param level Log level to use.
    * @param pFmt printf() style format string.
    * @param args va_list corresponding to pFmt.
    *******************************************************************************
    */
    void VLogText(LogLevel_t level,
                  const char *pFmt, va_list args) const PMS_VLOGTEXT_FMT_CHK
    {
#ifdef PMLOGLIB_3
        PmLogDebug(context_,pFmt,args);
#else
        PmLogVPrint(context_, aPmLogLevels_[level], pFmt, args);
#endif

    }


private:
    /* PmLogLib will not log binary data larger than 1024 bytes; if we
     * specify a data size greater than 1024, no data gets logged
     * (not even the first 1024 bytes). This looks like a bug in PmLogLib.
     * To work around this, we cap the maximum length of a binary message
     * to 1024 bytes so we can be sure that at least 1024 bytes gets logged.
     */
    static const size_t kLoggerMaxBinaryMsgSize = 1024;

    /* We use the aPmLogLevels_ array to map our log levels
     * to corresponding PmLogLib log levels. aPmLogLevels_
     * is defined in LoggerImpl.cpp.
     */
    static const PmLogLevel aPmLogLevels_[kLogLevelLast];

    PmLogContext context_;
};


class FuncLogger {
public:
    FuncLogger(const Logger& rLogger, const char* pFunc)
        : rLogger_(rLogger),
          pFunc_(pFunc),
          pObj_(NULL)
    {
        rLogger_.Debug("%s: Entering", pFunc_);
    }

    FuncLogger(const Logger& rLogger, const char* pFunc, const void* pObj)
        : rLogger_(rLogger),
          pFunc_(pFunc),
          pObj_(pObj)
    {
        rLogger_.Debug("%s: %p: Entering", pFunc_, pObj_);
    }

    ~FuncLogger()
    {
        if (!pObj_) {
            rLogger_.Debug("%s: Leaving", pFunc_);
        } else {
            rLogger_.Debug("%s: %p: Leaving", pFunc_, pObj_);
        }
    }

private:
    const Logger& rLogger_;
    const char* pFunc_;
    const void* pObj_;
};


#define PMS_MEMFUNC_LOG(logger) \
            ((void)0);

#define PMS_FUNC_LOG(logger) \
            ((void)0);

#endif // PMS_LOGGERIMPL_H
