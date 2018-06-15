// @@@LICENSE
//
//      Copyright (c) 2017-2018 LG Electronics, Inc.
//
// Confidential computer software. Valid license from LG required for
// possession, use or copying. Consistent with FAR 12.211 and 12.212,
// Commercial Computer Software, Computer Software Documentation, and
// Technical Data for Commercial Items are licensed to the U.S. Government
// under vendor's standard commercial license.
//
// LICENSE@@@

#ifndef CLIENT_PMS_BLOCK_H
#define CLIENT_PMS_BLOCK_H

#include <glib.h>
#include <memory>

#include "IpcBase.h"
#include "log.h"
#include "PmsConfig.h"

/**
 * @class ClientBlock
 * @brief Defines the client block class.
 * @details
 */

class ClientBlock
{
public:
    ClientBlock(PmsConfig* pConfig, GMainLoop* loopData);

    virtual ~ClientBlock()
    {
        for(uint8_t i = 0; i < mIpc.size(); i++ )
            delete mIpc[i];

        mIpc.clear();

        delete mpLog;
        mpLog = nullptr;
    }

    PmsErrorCode_t Initialize();
    PmsErrorCode_t Deinitialize();
    PmsErrorCode_t Start();
    PmsErrorCode_t Stop();

private:
    PmsErrorCode_t InitializeIpc();
    PmsErrorCode_t DeinitializeIpc();

public:
    static const char* const kPmsBlockName;

private:
    std::vector<IpcBase*>    mIpc;
    GMainLoop*               mLoopData;
    Logger*                  mpLog;
    bool                     mIsInitialized;
};

#endif //CLIENT_PMS_BLOCK_H
