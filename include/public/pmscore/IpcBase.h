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

#ifndef IPC_BASE_H
#define IPC_BASE_H

#include <vector>
#include <glib.h>

#include "IpcInterfaceBase.h"
#include "log.h"
#include "PmsConfig.h"
#include "PmsErrors.h"

class IpcBase
{
public:

  static const char* const kPmsIpcName;
  static const char* const kPmsIpcType;

  virtual ~IpcBase() {};

  const std::string& GetType()
  {
     return mType;
  }

  /**
    * @brief Starts the ipc communication with an external client.
    *
    * Successful completion of this function implies
    * @li The Ipc Block is ready to process requests from client apps.
    * @note Before Start(), Initialize() needs to be called to make sure the Ipc Interfaces are
    * up
    */
  virtual PmsErrorCode_t Start(GMainLoop* loopData)
  {
    mIsStarted = true;
    return kPmsSuccess;
  }

  /**
    * @brief Stop the ipc communication with an external client.
    *
    * Successful completion of this function implies
    * @li The Ipc Interfaces are still up, but Pms is no longer processing any client requests
    * @note : to bring down the Ipc interfaces, use DeInitialize()
    */
  virtual PmsErrorCode_t Stop()
  {
    mIsStarted = false;
    return kPmsSuccess;
  }

  /**
   * @brief Brings up the Ipc interfaces.
   *
   * This needs to be called before Start().
   * Successful Completion implies
   * @li Ipc core has been inited using InitIpc()
   * @li Ipc interface handlers have been created using CreateHandlers()
   * @li Ipc interfaces have been created and brought up.
   */
  virtual PmsErrorCode_t Initialize();

  /**
   * @brief Brings down the Ipc interfaces.
   *
   * They can brought up again using Initialize().
   * Before Deinitialize is called, Stop() needs to be called.
   * Successful completion implies
   * @li Ipc interface itself has been brought down using DeinitIpc().
   * @li Ipc Interface handlers have been destroyed using DestroyHandlers().
   */
  virtual PmsErrorCode_t Deinitialize();

protected:
    IpcBase(PmsConfig* pConfig, const std::string pType):
    mpConfig(pConfig), mType(pType)
    {
        std::string logContext;
        mIsInitialized = false;
        mIsStarted = false;
        mIpcInterfaces.clear();

        PmsErrorCode_t err = kPmsSuccess;
        const char* logCtxt = LOG_CONTEXT_DEBUG; //default logging context

        err = mpConfig->GetString(kPmsInterfaceName,
        kPmsLogContext, &logContext);

        if (err == kPmsSuccess)
        logCtxt = logContext.c_str();
        mpLog = new Logger(logCtxt);
    }

  /**
   * Brings up the core of IPC Communication
   * @note Called from Initialize()
   */
  virtual PmsErrorCode_t InitIpc() = 0;

  /**
   * Brings down the core of IPC Communication
   * @note Called from DeInitialize()
   */
  virtual PmsErrorCode_t DeinitIpc() = 0;

  /**
   * Queries the Pms Config file and creates Ipc Interface handler objects
   * from the appropriate Ipc Interface Object Factory
   * and adds them in the mIpcInterfaces Vector.
   * @note This is called from Initialize()
   */
  virtual PmsErrorCode_t CreateHandlers()  = 0;

  /**
   * Remove the IpcInterface objects from the mIpcInterfaces
   * Vector and destroy each IpcInterface object.
   * @note This is called from DeInitialize()
   */
  virtual PmsErrorCode_t DestroyHandlers();

  /*Ipc handlers*/
  std::vector<IpcInterfaceBase*> mIpcInterfaces;

  /**
   * Set to true on Start() and false on Stop(). Ensures validity of operations which can be
   * performed only after Start() and Stop().
   * @example DeInitialize() can be called only after Stop().
   */
  bool mIsStarted;

  /**
   * Set to true on Initialize() and false on DeInitialize().
   * Ensures validity of operations which can be  performed only after
   * Initialize(), i.e. Start()
   */
  bool mIsInitialized;
  /* Pms Config handle that can be used to read the configuration file*/
  PmsConfig* mpConfig;
  Logger* mpLog;
  //specifies the type of Ipcs this supports, i.e telephony, messaging, charging
  const std::string mType;

  static const char* const kPmsInterfaceName;
  static const char* const kPmsLogContext;
};
#endif
