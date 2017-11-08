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

#include <cstdlib>

#include "log.h"
#include "NyxUtil.h"

//As "nyx_system_reboot_type_t" is not defined in master branch, will replace this when
//nyx-lib is modified
typedef enum systemRebootTypes{
    SYSTEM_NORMAL_REBOOT,
    SYSTEM_RECOVERY_WIPE_DATA_REBOOT, // for factory reset
    SYSTEM_LAF_REBOOT,
} systemRebootType;

NyxUtil::NyxUtil()
{
    mNyxDeviceHandle = nullptr;
    init();
    mpLog = new Logger(LOG_CONTEXT_DEBUG);
}

void NyxUtil::init()
{
    int ret = NYX_ERROR_NONE;
    ret = nyx_device_open(NYX_DEVICE_SYSTEM, "Main", &mNyxDeviceHandle);

    if (ret != NYX_ERROR_NONE) {
        mNyxDeviceHandle = nullptr;
        MSG_DEBUG("Error while opening nyx device system. Error[%d]", ret);
        std::abort();
    }
}

time_t NyxUtil::getRTCTime()
{
    time_t now = 0;
    nyx_system_query_rtc_time(mNyxDeviceHandle, &now);
    return now;
}

NyxUtil &NyxUtil::getInstance()
{
    static NyxUtil handle;
    return handle;
}

void NyxUtil::shutdown(const std::string &reason)
{
    MSG_DEBUG("[%s] shutting down the system. reason[%s]", __FUNCTION__, reason.c_str());

    nyx_system_shutdown(mNyxDeviceHandle, NYX_SYSTEM_NORMAL_SHUTDOWN, reason.c_str());
}

// Check params, how to use
void NyxUtil::reboot(const std::string &reason)
{
    /*
    if (!reason.empty()) {
        if (reason == "recovery") {
            // Check params
            if (!params.empty()) {
                if (params == "wipe-data") {
                    nyx_system_erase_partition(mNyxDeviceHandle, NYX_SYSTEM_ERASE_ALL);
                }
            }
        }
    }*/

    //To make common to all devices, NYX_SYSTEM_EMERG_SHUTDOWN is used
    nyx_system_reboot(mNyxDeviceHandle, NYX_SYSTEM_EMERG_SHUTDOWN, reason.c_str());
}

void NyxUtil::setRtcAlarm()
{
    nyx_system_set_alarm(mNyxDeviceHandle, 0, nullptr, nullptr);
}
