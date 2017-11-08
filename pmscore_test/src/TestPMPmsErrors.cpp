/* @@@LICENSE
 *
 * Copyright (c) 2017 LG Electronics, Inc.
 *
 * Confidential computer software. Valid license from LG required for
 * possession, use or copying. Consistent with FAR 12.211 and 12.212,
 * Commercial Computer Software, Computer Software Documentation, and
 * Technical Data for Commercial Items are licensed to the U.S. Government
 * under vendor's standard commercial license.
 *
 * LICENSE@@@
 */

#include <gtest/gtest.h>
#include <pbnjson.hpp>

#include "PMSCommon.h"
#include "NyxUtil.h"
#include "TestUtil.h"
#include "PmsErrors.h"

TEST(TestPMPmsErrors, GetPmsErrorString)
{
    PmsErrorCode_t error;

    //! Test Case 1
    error = kPmsSuccess;
    EXPECT_STREQ(GetPmsErrorString(error), "success");
    //! Test Case 2
    error = kPmsErrPower2Busy;
    EXPECT_STREQ(GetPmsErrorString(error), "power2busy");
    //! Test Case 3
    error = kPmsErrInvalidJSONFormat;
    EXPECT_STREQ(GetPmsErrorString(error), "invalidjsonformat");
    //! Test Case 4
    error = kPmsErrClientNotRegistered;
    EXPECT_STREQ(GetPmsErrorString(error), "clientnotregistered");
    //! Test Case 5
    error = kPmsErrClientAlreadyRegistered;
    EXPECT_STREQ(GetPmsErrorString(error), "clientalreadyregistered");
    //! Test Case 6
    error = kPmsErrResponseTooLate;
    EXPECT_STREQ(GetPmsErrorString(error), "responsetoolate");
    //! Test Case 7
    error = kPmsErrTimerNotRunning;
    EXPECT_STREQ(GetPmsErrorString(error), "notrunning");
    //! Test Case 8
    error = kPmsErrUnknown;
    EXPECT_STREQ(GetPmsErrorString(error), "unknown");
}