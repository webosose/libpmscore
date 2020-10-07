// @@@LICENSE
//
//      Copyright (c) 2017-2020 LG Electronics, Inc.
//
// Confidential computer software. Valid license from LG required for
// possession, use or copying. Consistent with FAR 12.211 and 12.212,
// Commercial Computer Software, Computer Software Documentation, and
// Technical Data for Commercial Items are licensed to the U.S. Government
// under vendor's standard commercial license.
//
// LICENSE@@@

#include "PmsErrors.h"


const char* GetPmsErrorString(PmsErrorCode_t error)
{
    switch (error)
    {
    case kPmsSuccess:
        return "success";
    case kPmsErrPower2Busy:
        return "power2busy";
    case kPmsErrInvalidJSONFormat:
        return "invalidjsonformat";
    case kPmsErrClientNotRegistered:
        return "clientnotregistered";
    case kPmsErrClientAlreadyRegistered:
        return "clientalreadyregistered";
    case kPmsErrResponseTooLate:
        return "responsetoolate";
    case kPmsErrTimerNotRunning:
        return "notrunning";
    case kPmsErrUnknown:
        return "unknown";
    default:
        return "failure";
    }
}
