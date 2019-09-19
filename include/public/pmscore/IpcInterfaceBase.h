// @@@LICENSE
//
//      Copyright (c) 2019 LG Electronics, Inc.
//
// Confidential computer software. Valid license from LG required for
// possession, use or copying. Consistent with FAR 12.211 and 12.212,
// Commercial Computer Software, Computer Software Documentation, and
// Technical Data for Commercial Items are licensed to the U.S. Government
// under vendor's standard commercial license.
//
// LICENSE@@@

#ifndef IPC_INTERFACE_BASE_H
#define IPC_INTERFACE_BASE_H

#include "log.h"
#include "PmsConfigLinux.h"
#include "PmsErrors.h"

class IpcInterfaceBase
{
public:

  virtual ~IpcInterfaceBase()
  {
      if (mIsStarted)
      Stop();

      if (mIsInitialized)
      Deinitialize();

      delete mpLog;
      mpLog = nullptr;
  }

  virtual PmsErrorCode_t Initialize ()
  {
    PmsErrorCode_t err = kPmsSuccess;

    if (mIsInitialized)
      return err;
    else
      mIsInitialized = true;

    return err;
  }

  virtual PmsErrorCode_t Deinitialize ()
  {
    PmsErrorCode_t err = kPmsSuccess;

    if (!mIsInitialized)
      return err;
    else
      mIsInitialized = false;

    return err;
  }

  virtual PmsErrorCode_t Start ()
  {
    PmsErrorCode_t err = kPmsSuccess;

    if (mIsStarted)
      return err;
    else
      mIsStarted = true;

    return err;
  }

  virtual PmsErrorCode_t Stop ()
  {
    PmsErrorCode_t err = kPmsSuccess;

    if (!mIsStarted)
      return err;
    else
      mIsStarted = false;

    return err;
  }

protected:

  /**
   * Called from the destructor of the handlers to deregister all ipc commands
   * the handler can service
   */
  virtual PmsErrorCode_t UnregisterIpc() = 0;

  IpcInterfaceBase(PmsConfig* pConfig): mpConfig(pConfig)
  {
      std::string logContext;
      mIsInitialized = false;
      mIsStarted = false;

      PmsErrorCode_t err = kPmsSuccess;
      const char* logCtxt = LOG_CONTEXT_DEBUG; //default logging context

      err = mpConfig->GetString(kPmsInterfaceName,
      kPmsLogContext, &logContext);

      if (err == kPmsSuccess)
      logCtxt = logContext.c_str();
      mpLog = new Logger(logCtxt);
  }

  //Check if handler is initialized
  bool mIsInitialized;
  //Check if handler is started
  bool mIsStarted;

  //handle to the config file
  PmsConfig* mpConfig;

  Logger* mpLog;

  const char* const kPmsInterfaceName = "LunaInterfacePMSRoot";
  const char* const kPmsLogContext = "LogContext";
private:

};

#endif
