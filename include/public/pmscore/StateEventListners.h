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

#ifndef STATE_EVENT_LISTENER_H
#define STATE_EVENT_LISTENER_H

#include <string>

class StateEventListners
{
public:
    virtual ~StateEventListners() = default;
    virtual bool handleStateChange(const std::string& statename) = 0;
    virtual bool handleTransitionState(const std::string& processing) = 0;
};

#endif //STATE_EVENT_LISTENER_H
