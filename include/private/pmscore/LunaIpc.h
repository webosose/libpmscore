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

#ifndef LUNA_IPC_H
#define LUNA_IPC_H

#include <luna-service2/lunaservice.hpp>

#include "log.h"
#include "IpcBase.h"
#include "IpcFactory.h"

class LunaIpc : public IpcBase
{

public:

  static const char* const kPmsIpcName;

  static const char* const kPmsLunaPMS;
  static const char* const kPmsLunaServiceName;
  static const char* const kPmsLunaInterface;

 /**
  * @brief A Luna Ipc object can be created by using this method only.
  * This function is registered with the LunaIpc Object factory along with an unique Id.
  */
  static LunaIpc* CreateObject(PmsConfig* pConfig,
                               const std::string pType)
  {
     return new LunaIpc(pConfig, pType);
  }

  /* destructor*/
  virtual ~LunaIpc();

  /* Implements the Start() method from the base class*/
  virtual PmsErrorCode_t Start(GMainLoop* loopData);
  /* Implements the Stop() method from the base class*/
  virtual PmsErrorCode_t Stop();

protected:

  LunaIpc(PmsConfig* pConfig, const std::string pType):
         IpcBase(pConfig, pType)
  {
      std::string logContext;

      PmsErrorCode_t err = kPmsSuccess;
      const char* logCtxt = LOG_CONTEXT_DEBUG; //default logging context

      err = mpConfig->GetString(kPmsInterfaceName,
      kPmsLogContext, &logContext);

      if (err == kPmsSuccess)
          logCtxt = logContext.c_str();

      mpLog = new Logger(logCtxt);
      mpLsHandle = 0;
  }

   /**
   * Brings up the core of IPC Communication
   * @note Called from Initialize()
   */
  virtual PmsErrorCode_t InitIpc();

  /**
   * Brings down the core of IPC Communication
   * @note Called from DeInitialize()
   */
  virtual PmsErrorCode_t DeinitIpc();

  /**
   * Queries the Pms Config file and creates Ipc Interface handler objects
   * from the appropriate Ipc Interface Object Factory
   * and adds them in the mIpcInterfaces Vector.
   * @note This is called from Initialize()
   */
  virtual PmsErrorCode_t CreateHandlers();

private:

  /**
   * @brief Register the Create() method of the LunaIpc class with the LunaIpc Object factory
   *
   * This registration happens automatically when the Pms library is loaded*/
  static bool RegisterObject()
  {
    return sIpcFactory::GetInstance().Register(LunaIpc::kPmsLunaPMS, &LunaIpc::CreateObject);
  }

  /*@var Used to Check if the registration operation to the object factory has
   * succeeded. This variable is assigned to the method Register()
   */
  static bool mIsObjRegistered;

  /**
   * Luna handle
   */
  LSHandle* mpLsHandle;

  //The name under which the clients can access the interfaces registered with this ipc
  std::string mServiceName;

  Logger* mpLog;

  const char* const kPmsInterfaceName = "LunaInterfacePMSRoot";
  const char* const kPmsLogContext = "LogContext";

  static bool CancelSubscriptionCb(LSHandle* pLsH, LSMessage* pLsMsg, void* pData);

};

#endif


