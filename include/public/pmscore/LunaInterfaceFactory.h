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

#ifndef LUNA_INTERFACE_FACTORY_H
#define LUNA_INTERFACE_FACTORY_H

#include <functional>
#include <luna-service2/lunaservice.hpp>

#include "IpcInterfaceBase.h"
#include "PmsObjectFactory.h"
#include "Singleton.h"

typedef std::function<IpcInterfaceBase* (PmsConfig*, LSHandle*)> funcLunaInterfaceCreator;

typedef Singleton<
ObjectFactory<IpcInterfaceBase, std::string, funcLunaInterfaceCreator, PmsConfig*, LSHandle*> >
    sLunaInterfaceFactory;

#endif
