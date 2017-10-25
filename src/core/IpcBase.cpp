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

#include "IpcBase.h"

/** @brief Definition of all static class members
 */
const char* const IpcBase::kPmsIpcName = "Ipc";
const char* const IpcBase::kPmsIpcType = "Type";

const char* const IpcBase::kPmsInterfaceName = "LunaInterfacePMSRoot";
const char* const IpcBase::kPmsLogContext = "LogContext";

PmsErrorCode_t IpcBase::Initialize()
{
  PmsErrorCode_t err = kPmsSuccess;

  if (mIsInitialized)
      return err;

  err = InitIpc(); // will call lunaIPC initipc

  if (err != kPmsSuccess)
  {
      MSG_ERROR(err, "InitializeIpc() Failed!");
      return err;
  }

  err = CreateHandlers();   //will call lunaIPC CreateHandlers
  if (err != kPmsSuccess)
  {
      MSG_ERROR(err, "CreateHandlers() Failed!");
      return err;
  }

  mIsInitialized = true;

  return err;
}


PmsErrorCode_t IpcBase::Deinitialize()
{
  PmsErrorCode_t err = kPmsSuccess;

  if (!mIsInitialized)
      return err;

  err = DestroyHandlers();
  if (err != kPmsSuccess)
  {
      MSG_ERROR(err, "DestroyHandlers() Failed!");
      return err;
  }

  err = DeinitIpc();
  if (err != kPmsSuccess)
  {
      MSG_ERROR(err, "DeinitializeIpc() Failed!");
      return err;
  }

  mIsInitialized = false;

  return err;
}

PmsErrorCode_t IpcBase::DestroyHandlers()
{
  PmsErrorCode_t err = kPmsSuccess;

  for(uint8_t i = 0; i < mIpcInterfaces.size(); i++ )
  {
    delete mIpcInterfaces[i];
  }

  mIpcInterfaces.clear();

  return err;
}
