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

#ifndef PMS_LOGGING_H
#define PMS_LOGGING_H

#include <glib.h>
#include <PmLogLib.h>
#include <typeinfo>
#include <stdio.h>

#include "LoggerImpl.h"
#include "PmsErrors.h"

#define PMS_CLASS      (typeid(*this).name()+2)
#define PMS_FUNCTION   __func__
#define STR_LEN 512
#define DUP(x) #x

static PmLogContext pmloglib_ctx = kPmLogGlobalContext;

static const char* CONTEXT_NAME = "powermgr";

class PmsLog
{
private:
    PmsLog() { };
    ~PmsLog() { };

public:
    static void log_context_init()
    {
        if (pmloglib_ctx == NULL || pmloglib_ctx == kPmLogGlobalContext)
        {
            PmLogErr Pmerr = PmLogGetContext(CONTEXT_NAME, &pmloglib_ctx);
            if (Pmerr != kPmLogErr_None)
            {
                pmloglib_ctx = kPmLogGlobalContext;
            }
        }
    }

    static void PmsDebugLog(const Logger *pLogger, const char *PmsClass, const char *PmsFunc, const char *format, ...)
    {
        if (pLogger->IsEnabled(Logger::kLogLevelDebug))
        {
            va_list args;
            va_start (args, format);

            char buffer[STR_LEN]={0};
            int len = snprintf(buffer, sizeof(buffer), "%s::%s ", PmsClass, PmsFunc);
            len = vsnprintf(&buffer[len], sizeof(buffer) - len, format, args);

            pLogger->Debug(buffer);

            va_end (args);
        }
    }

    static void PmsNoticeLog(const Logger *pLogger, const char *PmsClass, const char *PmsFunc, const char *format, ...)
    {
        if (pLogger->IsEnabled(Logger::kLogLevelNotice))
        {
            va_list args;
            va_start (args, format);
            char buffer[STR_LEN] = {0};
            int len = snprintf(buffer, sizeof(buffer), "%s::%s ", PmsClass, PmsFunc);
            len = vsnprintf(&buffer[len], sizeof(buffer) - len, format, args);

            pLogger->Notice(buffer);

            va_end (args);
        }
    }

    static void PmsInfoLog(const Logger *pLogger, const char *PmsClass, const char *PmsFunc, const char *format, ...)
    {
        if (pLogger->IsEnabled(Logger::kLogLevelInfo))
        {
            va_list ar;
            va_start (ar, format);
            char buffer[STR_LEN] = {0};
            int len = snprintf(buffer, sizeof(buffer), "%s::%s ", PmsClass, PmsFunc);
            len = vsnprintf(&buffer[len], sizeof(buffer)-len, format, ar);

            pLogger->Info(buffer);

            va_end (ar);
        }
    }

    static void PmsWarningLog(const Logger *pLogger, const char *PmsClass, const char *PmsFunc, const char *format, ...)
    {
        if (pLogger->IsEnabled(Logger::kLogLevelWarning))
        {
            va_list ar;
            va_start (ar, format);
            char buffer[STR_LEN] = {0};
            int len = snprintf(buffer, sizeof(buffer), "%s::%s ", PmsClass, PmsFunc);
            len = vsnprintf(&buffer[len], sizeof(buffer)-len, format, ar);

            pLogger->Warning(buffer);

            va_end (ar);
        }
    }

    static void PmsErrorLog(const Logger *pLogger, PmsErrorCode_t err, const char *errString, const char *PmsClass,
                                    const char *PmsFunc, const char *format, ...)
    {
        if (pLogger->IsEnabled(Logger::kLogLevelError))
        {
            va_list ar;
            va_start (ar, format);
            char buffer[STR_LEN] = {0};
            int len = snprintf(buffer, sizeof(buffer), "%s::%s Error=%s(%d) ", PmsClass, PmsFunc, errString, err);
            len = vsnprintf(&buffer[len], sizeof(buffer)-len, format, ar);

            pLogger->Error(buffer);

            va_end (ar);
        }
    }

    static void PmsLogNonClass(const char *category , int level, const char *format, PmsErrorCode_t err, ...)
    {
        Logger *pLogger = new Logger(category);
        va_list ar;
        va_start (ar, err);
        char buffer[STR_LEN] = {0};
        vsnprintf(buffer, sizeof(buffer), format, ar);
        va_end(ar);
        switch(level)
        {
            case kPmLogLevel_Error   : PmsErrorLog(pLogger, err, DUP(err),"", PMS_FUNCTION,buffer, NULL);
                                       break;
            case kPmLogLevel_Notice  : PmsNoticeLog(pLogger, "", PMS_FUNCTION,buffer, NULL);
                                       break;
            case kPmLogLevel_Info    : PmsInfoLog(pLogger, "", PMS_FUNCTION,buffer, NULL);
                                       break;
            case kPmLogLevel_Warning : PmsWarningLog(pLogger, "", PMS_FUNCTION,buffer, NULL);
                                       break;
            case kPmLogLevel_Debug   : PmsDebugLog(pLogger, "", PMS_FUNCTION, buffer, NULL);
                                       break;
            default:                   break;
        }
        delete(pLogger);
    }
};

#endif // PMS_LOGGING_H

