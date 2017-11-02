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

#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

/*Added to get instance from FSM*/

#include <functional>
#include <pbnjson.hpp>
#include <set>
#include <string>
#include <utility>

#include "FiniteStateMachine.h"
#include "FSM.h"
#include "log.h"
#include "PmsConfigLinux.h"
#include "Singleton.h"
#include "StateEventListners.h"

class StateManager {
public:
    bool init();
    bool deinit();

    static FSM::CFiniteStateMachine* getFSMInstance() {
        if (nullptr == mFsm) {
            static FSM::CFiniteStateMachine obj;
            mFsm = &obj;
        }

        return mFsm;
    }

    bool registerListener(StateEventListners *l);
    bool unregisterListener(StateEventListners *l);
    bool processEvent(const std::string& eventName);

    ~StateManager() {

        delete mpConfig;
        mpConfig = nullptr;

        delete mpLog;
        mpLog = nullptr;
    }

    bool notifyStateListeners(const std::string& statename);
    bool notifyTransitionStateListeners(const std::string& processing);
    bool requestToSetState(const std::string& stateName);
    std::string requestToGetState();

    const char* const kPmsInterfaceName = "LunaInterfacePMSRoot";
    const char* const kPmsLogContext = "LogContext";

    StateManager()
    {
        std::string logContext;
        isStateMachineRunning = false;

        mpConfig = new PmsConfigLinux(PMS_CONF_FILE_PATH);
        PmsErrorCode_t err = kPmsSuccess;
        const char* logCtxt = LOG_CONTEXT_DEBUG; //default logging context

        err = mpConfig->GetString(kPmsInterfaceName,
            kPmsLogContext, &logContext);

        if (err == kPmsSuccess)
            logCtxt = logContext.c_str();

        mpLog = new Logger(logCtxt);

        MSG_DEBUG("State Manager Constructed");
    }

private:
    std::set<StateEventListners *> mListeners;
    static FSM::CFiniteStateMachine* mFsm;
    bool isStateMachineRunning;

    Logger* mpLog;
    PmsConfig* mpConfig;
};

typedef Singleton<StateManager> stateReference;

#endif
