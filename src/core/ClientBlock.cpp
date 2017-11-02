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

#include <typeinfo>
#include <memory>

#include "ClientBlock.h"
#include "IpcFactory.h"

const char* const ClientBlock::kPmsBlockName = "PMS";

ClientBlock::ClientBlock(PmsConfig* pConfig, GMainLoop* loopData):
    mLoopData(loopData)
{
    std::vector<std::string> ipcConfig;
    std::vector<std::string> ipcTypeConfig;
    PmsErrorCode_t err = kPmsSuccess;

    mpLog = new Logger(LOG_CONTEXT_CLIENT);

    MSG_DEBUG("Logger Instance created");

    //Get the types of Ipc Supported
    err = pConfig->GetStringList(ClientBlock::kPmsBlockName, IpcBase::kPmsIpcName, &ipcConfig);
    for(uint8_t i = 0; i < ipcConfig.size(); i++)
    {
        ipcTypeConfig.clear();

        //For each ipc, get the specific ipc flavours supported and create them
        pConfig->GetStringList(ipcConfig[i], IpcBase::kPmsIpcType, &ipcTypeConfig);

        for(uint8_t j = 0; j < ipcTypeConfig.size(); j++)
        {
            IpcBase* pIpc = sIpcFactory::GetInstance().CreateObject(ipcTypeConfig[j], pConfig,
                                                                    ipcTypeConfig[j]);

            if (pIpc)
            {
                mIpc.push_back(pIpc);
                MSG_DEBUG("Created object %s", ipcTypeConfig[j].c_str());
            }
            else
                MSG_DEBUG("Could not create object %s", ipcTypeConfig[j].c_str());
        }
    }
}

PmsErrorCode_t ClientBlock::Initialize()
{
    PmsErrorCode_t err = kPmsSuccess;
    err = InitializeIpc();
    if (err != kPmsSuccess)
    {
        return err;
    }

    mIsInitialized = true;

    return err;
}

PmsErrorCode_t ClientBlock::Start()
{
    PmsErrorCode_t err = kPmsSuccess;
    // to attach at gmainloop
    for(uint8_t i = 0; i < mIpc.size(); i++ )
    {
        err = mIpc[i]->Start(mLoopData);
    }
        return err;
}

PmsErrorCode_t ClientBlock::Stop()
{
    PmsErrorCode_t err = kPmsSuccess;

    for(uint8_t i = 0; i < mIpc.size(); i++ )
    {
        err = mIpc[i]->Stop();
    }

    return err;
}

PmsErrorCode_t ClientBlock::Deinitialize()
{
    MSG_DEBUG("[%s]", __PRETTY_FUNCTION__);
    PmsErrorCode_t err = kPmsSuccess;
    err = DeinitializeIpc();
    if (err != kPmsSuccess)
    {
        return err;
    }

    mIsInitialized = false;

    return err;
}

PmsErrorCode_t ClientBlock::InitializeIpc()
{
    MSG_DEBUG("[%s]", __PRETTY_FUNCTION__);
    PmsErrorCode_t err = kPmsSuccess;

    for(uint8_t i = 0; i < mIpc.size(); i++ )
    {
        err = mIpc[i]->Initialize();
        if (err != kPmsSuccess)
        {
            MSG_ERROR(err, "Ipc not initialized");
            return err;
        }
    }

    return kPmsSuccess;
}

PmsErrorCode_t ClientBlock::DeinitializeIpc()
{
    MSG_DEBUG("[%s]", __PRETTY_FUNCTION__);

    PmsErrorCode_t err = kPmsSuccess;

    for(uint8_t i = 0; i < mIpc.size(); i++ )
    {
        err = mIpc[i]->Deinitialize();
    }
    return err;
}
