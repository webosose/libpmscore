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

#include "LoggerImpl.h"

#ifdef PMLOGLIB_3
const PmLogLevel Logger::aPmLogLevels_[kLogLevelLast] = {
    kPmLogLevel_Critical,
    kPmLogLevel_Critical,
    kPmLogLevel_Critical,
    kPmLogLevel_Error,
    kPmLogLevel_Warning,
    kPmLogLevel_Info,
    kPmLogLevel_Info,
    kPmLogLevel_Debug
};
#else
const PmLogLevel Logger::aPmLogLevels_[kLogLevelLast] = {
    kPmLogLevel_Emergency,
    kPmLogLevel_Alert,
    kPmLogLevel_Critical,
    kPmLogLevel_Error,
    kPmLogLevel_Warning,
    kPmLogLevel_Notice,
    kPmLogLevel_Info,
    kPmLogLevel_Debug
};
#endif
