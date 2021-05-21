// @@@LICENSE
//
//      Copyright (c) 2017-2021 LG Electronics, Inc.
//
// Confidential computer software. Valid license from LG required for
// possession, use or copying. Consistent with FAR 12.211 and 12.212,
// Commercial Computer Software, Computer Software Documentation, and
// Technical Data for Commercial Items are licensed to the U.S. Government
// under vendor's standard commercial license.
//
// LICENSE@@@

#ifndef LUNA_INTERFACE_BASE_H
#define LUNA_INTERFACE_BASE_H

#include "IpcInterfaceBase.h"
#include "log.h"
#include "LunaInterfaceFactory.h"
#include "PmsConfigLinux.h"

typedef std::function<bool (LSMessage* pLsMsg)> CancelSubscriptionFunction_t;
typedef std::map<LSMessage*, CancelSubscriptionFunction_t> CancelSubscriptionTable_t;

class LunaInterfaceBase: public IpcInterfaceBase
{
public:
  virtual ~LunaInterfaceBase()
  {
    mpLsHandle = 0;
  }

  bool CancelSubscription(LSHandle* pLsH, LSMessage* pLsMsg, void* pData);

protected:

LunaInterfaceBase(PmsConfig* pConfig, LSHandle* pHandle): IpcInterfaceBase(pConfig), mpLsHandle(pHandle)
  {
      std::string logContext;

      PmsErrorCode_t err = kPmsSuccess;
      const char* logCtxt = LOG_CONTEXT_DEBUG; //default logging context

      err = pConfig->GetString(kPmsInterfaceName,
      kPmsLogContext, &logContext);

      if (err == kPmsSuccess)
      logCtxt = logContext.c_str();
      mpLog = new Logger(logCtxt);
  };

  virtual void RegisterIpc(LSHandle* pHandle) = 0;
  virtual PmsErrorCode_t UnregisterIpc()
  {
    return kPmsSuccess;
  }

  LSHandle* mpLsHandle;
  bool RegisterCancelSubscriptionCallback(LSMessage* pLsMsg, const CancelSubscriptionFunction_t& callback);

private:
  static CancelSubscriptionTable_t mCancelSubscriptionTable;
  Logger* mpLog;

  const char* const kPmsInterfaceName = "LunaInterfacePMSRoot";
  const char* const kPmsLogContext = "LogContext";
};

#endif


