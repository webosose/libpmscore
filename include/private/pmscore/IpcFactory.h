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

#ifndef IPC_FACTORY_H
#define IPC_FACTORY_H

#include <functional>

#include "IpcBase.h"
#include "PmsObjectFactory.h"
#include "Singleton.h"

class PmsConfig;

typedef std::function<IpcBase* (PmsConfig*, const std::string )> funcIpcCreator;
typedef Singleton<ObjectFactory<IpcBase, std::string, funcIpcCreator, PmsConfig*,
        const std::string> >  sIpcFactory;
#endif
