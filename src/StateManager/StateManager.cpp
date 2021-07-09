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

#include <algorithm>
#include <cstring>
#include <errno.h>
#include <string>
#include <unistd.h>

#include "FSMErrorCodes.h"
#include "FSMLoader.h"
#include "PMSCommon.h"
#include "PmsLogging.h"
#include "StateManager.h"

using namespace FSM;

CFiniteStateMachine* StateManager::mFsm = nullptr;

bool StateManager::init()
{
    mFsm = getFSMInstance();

    CFSMSimpleLoaderXML* loader = nullptr;
    if(mFsm != NULL)
        loader = new CFSMSimpleLoaderXML(mFsm);

    char  fileName[1024] = FSM_FILE_PATH;
    if (!loader->Load(fileName))
    {
        iHandleLastError();
    }
    else if(!mFsm->Start())
    {
         iHandleLastError();
    }
    else
    {
        isStateMachineRunning = true;
    }

    delete loader;
    return isStateMachineRunning;
}

bool StateManager::deinit() {
    if(isStateMachineRunning) {
        mFsm->Stop();
        isStateMachineRunning = false;
        MSG_INFO("State Machine Stopped");
    }

    return true;
}

bool StateManager::processEvent(const std::string& eventName)
{
    if(!isStateMachineRunning) {
        return false;
    }

    MSG_DEBUG("Process Event: %s", eventName.c_str());

    if (!mFsm->ProcessEvent(eventName.c_str(),0,0)) {
        iHandleLastError();
        return false;
    }

    return true;
}

bool StateManager::notifyStateListeners(const std::string& statename)
{
    bool retVal = false;
    std::for_each(mListeners.begin(), mListeners.end(), [&](StateEventListners * l)
    {
        retVal = l->handleStateChange(statename);
        if(!retVal)
        {
            MSG_DEBUG("Notification failed!");
        }
    });

    return retVal;
}

bool StateManager::notifyTransitionStateListeners(const std::string& nextState)
{
    bool retVal = false;
    std::for_each(mListeners.begin(), mListeners.end(), [&](StateEventListners * l)
    {
        retVal = l->handleTransitionState(nextState);
        if(!retVal)
        {
            MSG_DEBUG("Notification failed!");
        }
    });

    return retVal;
}

bool StateManager::registerListener(StateEventListners *l)
{
    MSG_INFO("Adding Listener");
    if (!l) {
        return false;
    }
    mListeners.insert(l);
    return true;
}

bool StateManager::unregisterListener(StateEventListners *l)
{
    MSG_INFO("Removing Listener");
    if (!l) {
        return false;
    }

    mListeners.erase(l);
    return true;
}

bool StateManager::requestToSetState(const std::string& stateName)
{
    if(!isStateMachineRunning) {
        return false;
    }

    CFSMState *State = nullptr;
    State = mFsm->GetState(stateName.c_str());
    if (State == NULL)
    {
        MSG_INFO("State is Undefined !!!");
        return false;
    }

    mFsm->SwitchToState(State);
    MSG_DEBUG("SwitchedToState  --> state = %s", State->GetName());
    return true;
}

std::string StateManager::requestToGetState()
{
    if(!isStateMachineRunning) {
        return "undefined";
    }

    CFSMState *State = nullptr;
    State = mFsm->GetCurrentState();

    if (NULL == State)
    {
        MSG_INFO("State is Undefined !!!");
        return "undefined";
    }

    std::string state = State->GetName();
    return state;
}
