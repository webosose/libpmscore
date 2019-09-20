// @@@LICENSE
//
//      Copyright (c) 2017-2019 LG Electronics, Inc.
//
// Confidential computer software. Valid license from LG required for
// possession, use or copying. Consistent with FAR 12.211 and 12.212,
// Commercial Computer Software, Computer Software Documentation, and
// Technical Data for Commercial Items are licensed to the U.S. Government
// under vendor's standard commercial license.
//
// LICENSE@@@


#include "LunaInterfaceBase.h"
#include "LunaIpc.h"
#include "LunaInterfaceFactory.h"

/** @brief Definition of all static class members
 */
const char* const LunaIpc::kPmsIpcName = "Luna";
const char* const LunaIpc::kPmsLunaPMS  = "LunaPMS";
const char* const LunaIpc::kPmsLunaServiceName = "ServiceName";
const char* const LunaIpc::kPmsLunaInterface  = "Interface";

bool LunaIpc::mIsObjRegistered = LunaIpc::RegisterObject();

PmsErrorCode_t LunaIpc::InitIpc()
{
    PmsErrorCode_t err = kPmsSuccess;
    err = mpConfig->GetString(mType, LunaIpc::kPmsLunaServiceName, &mServiceName);

    if (err == kPmsSuccess)
    {
        LSError lserror;
        LSErrorInit(&lserror);

        if (!LSRegister(mServiceName.c_str(), &mpLsHandle, &lserror))
        return kPmsErrClientNotRegistered;

        if (LSErrorIsSet(&lserror))
        {
            err = kPmsErrResponseTooLate;
            MSG_ERROR(err, "Could not register pms service name %s", mServiceName.c_str());
            MSG_DEBUG("LUNASERVICE ERROR %d: %s (%s @ %s:%d)\n", lserror.error_code, lserror.message, lserror.func, lserror.file, lserror.line);
        }
        else
        {
            MSG_NOTICE("Service %s registered, mpLsHandle 0x%x", mServiceName.c_str(), mpLsHandle);
        }
    }

    return err;
}

PmsErrorCode_t LunaIpc::DeinitIpc()
{
    LSError lserror;
    LSErrorInit(&lserror);

    PmsErrorCode_t err = kPmsSuccess;

    LSUnregister(mpLsHandle, &lserror);

    if (LSErrorIsSet(&lserror))
    {
        MSG_DEBUG("LUNASERVICE ERROR %d: %s (%s @ %s:%d)\n", lserror.error_code, lserror.message, lserror.func, lserror.file, lserror.line);
        return kPmsErrResponseTooLate;
    }
    return kPmsSuccess;
}

LunaIpc::~LunaIpc()
{
    if (mIsStarted)
        Stop();

    if (mIsInitialized)
        Deinitialize();

    mpLsHandle = 0;
}

PmsErrorCode_t LunaIpc::CreateHandlers()
{
    PmsErrorCode_t err = kPmsSuccess;
    std::vector<std::string> lunaInterfaces;

    err = mpConfig->GetStringList(mType, LunaIpc::kPmsLunaInterface, &lunaInterfaces);

    if (err == kPmsSuccess)
    {
        for(uint8_t i = 0; i < lunaInterfaces.size(); i++)
        {
            IpcInterfaceBase* pIpcInterface =
                sLunaInterfaceFactory::GetInstance().CreateObject(lunaInterfaces[i], mpConfig, mpLsHandle);

            if (pIpcInterface)
            {
                mIpcInterfaces.push_back(pIpcInterface);
                MSG_DEBUG("%s created", lunaInterfaces.at(i).c_str());
            }
            else
            {
                MSG_ERROR(kPmsErrUnknown, "%s could not be created", lunaInterfaces.at(i).c_str());
            }
        }
    }
    return err;
}

PmsErrorCode_t LunaIpc::Start(GMainLoop* loopData)
{
    LSError lserror;
    LSErrorInit(&lserror);

    PmsErrorCode_t err = kPmsSuccess;

    IpcBase::Start(loopData);

    LSGmainAttach( mpLsHandle, loopData, &lserror);
    if (!mIpcInterfaces.empty())
    {
        LSSubscriptionSetCancelFunction(mpLsHandle, CancelSubscriptionCb, (void*)mIpcInterfaces[0], NULL);
    }

    for (uint8_t i=0; i<mIpcInterfaces.size(); i++)
    {
        err = ((IpcInterfaceBase *)mIpcInterfaces[i])->Start(); // this to attach observers
    }

    return err;
}

PmsErrorCode_t LunaIpc::Stop()
{
    PmsErrorCode_t err = kPmsSuccess;

    LSError lserror;
    LSErrorInit(&lserror);

    for (uint8_t i=0; i<mIpcInterfaces.size(); i++)
    {
        err = ((IpcInterfaceBase *)mIpcInterfaces[i])->Stop();
    }

    IpcBase::Stop();

    return err;
}

bool
LunaIpc::CancelSubscriptionCb(LSHandle* pLsH, LSMessage* pLsMsg, void* pData)
{
    LunaInterfaceBase *pThis = static_cast<LunaInterfaceBase*>(pData);

    if (pThis != NULL)
    {
        return pThis->CancelSubscription(pLsH, pLsMsg, pData);
    }
    else
    {
        return false;
    }
}

