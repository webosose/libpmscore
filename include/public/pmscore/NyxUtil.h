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

#ifndef UTIL_NYXUTIL_H
#define UTIL_NYXUTIL_H

#include <time.h>
#include <string>
#include <nyx/nyx_client.h>

#include"LoggerImpl.h"

class NyxUtil
{
    public:
        ~NyxUtil()
        {
            delete mpLog;
            mpLog = nullptr;
        }
        time_t getRTCTime();
        static NyxUtil &getInstance();

        void shutdown(const std::string &reason);
        // TODO: check the params. why it was required, in earlier declaration
        // void reboot(const std::string &reason, const std::string &params);
        void reboot(const std::string &reason);
        void setRtcAlarm();

    private:
        void init();
        NyxUtil();

    private:
        nyx_device_handle_t mNyxDeviceHandle;
        Logger* mpLog;
};

#endif /* UTIL_NYXUTIL_H */

