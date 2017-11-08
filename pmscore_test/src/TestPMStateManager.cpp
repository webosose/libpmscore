/* @@@LICENSE
 *
 * Copyright (c) 2017 LG Electronics, Inc.
 *
 * Confidential computer software. Valid license from LG required for
 * possession, use or copying. Consistent with FAR 12.211 and 12.212,
 * Commercial Computer Software, Computer Software Documentation, and
 * Technical Data for Commercial Items are licensed to the U.S. Government
 * under vendor's standard commercial license.
 *
 * LICENSE@@@
 */

#include <gtest/gtest.h>
#include <pbnjson.hpp>

#include "PMSCommon.h"
#include "NyxUtil.h"
#include "TestUtil.h"
#include "StateManager.h"
#include "PmsConfigLinux.h"
#include "PmsErrors.h"

class TestPMStateManager: public ::testing::Test {
    public:
        TestPMStateManager()
        {
            printf("TestPMStateManager constructor entered\n");
            printf("TestPMStateManager constructor exit\n");
        }

        ~TestPMStateManager() {
            printf("====TestPMStateManager destructor entered ===\n");
            printf("====TestPMStateManager destructor exit ===\n");
        }
};

TEST_F(TestPMStateManager, StateManager_init)
{
    printf("------------------ BEGIN StateManager_init ----------------------------\n");
    StateManager *ptrSM= new StateManager();
    //! Test Case 1
    EXPECT_FALSE(ptrSM->init());
    printf("------------------ END StateManager_init----------------------------\n");
}

TEST_F(TestPMStateManager, StateManager_deinit)
{
    printf("------------------ BEGIN StateManager_deinit ----------------------------\n");
    StateManager *ptrSM= new StateManager();
    //! Test Case
    EXPECT_TRUE(ptrSM->deinit());
    printf("------------------ END StateManager_deinit----------------------------\n");
}

TEST_F(TestPMStateManager, StateManager_processEvent)
{
    printf("------------------ BEGIN StateManager_processEvent ----------------------------\n");
    StateManager *ptrSM= new StateManager();
    //! Test Case 1
    const std::string eventName1 = "poweroff";
    //! State Machine not running
    EXPECT_FALSE(ptrSM->processEvent(eventName1));
    const std::string eventName2 = "sleep";
    //! Unknown event
    EXPECT_FALSE(ptrSM->processEvent(eventName2));
    printf("------------------ END StateManager_processEvent----------------------------\n");
}

TEST_F(TestPMStateManager, StateManager_notifyStateListeners)
{
    printf("------------------ BEGIN StateManager_notifyStateListeners ----------------------------\n");
    StateManager *ptrSM= new StateManager();
    //! Test Case 1
    const std::string stateName1 = "ActiveStandbyState";
    //! State Machine not running
    EXPECT_FALSE(ptrSM->notifyStateListeners(stateName1));
    //! Unknown State
    const std::string stateName2 = "InitState";
    EXPECT_FALSE(ptrSM->notifyStateListeners(stateName2));
    printf("------------------ END StateManager_notifyStateListeners----------------------------\n");
}

TEST_F(TestPMStateManager, StateManager_notifyTransitionStateListeners)
{
    printf("------------------ BEGIN StateManager_notifyTransitionStateListeners ----------------------------\n");
    StateManager *ptrSM= new StateManager();
    //! Test Case 1
    const std::string stateName1 = "PowerOffState";
    //! State Machine not running
    EXPECT_FALSE(ptrSM->notifyTransitionStateListeners(stateName1));
    const std::string stateName2 = "InitState";
    //! Unknown State
    EXPECT_FALSE(ptrSM->notifyTransitionStateListeners(stateName2));
    printf("------------------ END StateManager_notifyTransitionStateListeners----------------------------\n");
}

TEST_F(TestPMStateManager, StateManager_requestToSetState)
{
    printf("------------------ BEGIN StateManager_requestToSetState ----------------------------\n");
    StateManager *ptrSM= new StateManager();
    //! Test Case 1
    const std::string stateName1 = "PowerOffState";
    //! State Machine not running
    EXPECT_FALSE(ptrSM->requestToSetState(stateName1));
    const std::string stateName2 = "InitState";
    //! Undefined State
    EXPECT_FALSE(ptrSM->requestToSetState(stateName2));
    printf("------------------ END StateManager_requestToSetState----------------------------\n");
}

TEST_F(TestPMStateManager, StateManager_requestToGetState)
{
    printf("------------------ BEGIN StateManager_requestToGetState ----------------------------\n");
    bool nameCheck = false;
    StateManager *ptrSM= new StateManager();
    //! Test Case 1
    std::string stateName = ptrSM->requestToGetState();
    if(strcmp(stateName.c_str(), "ActiveState") == 0)
        nameCheck = true;
    //! State Machine not running
    EXPECT_FALSE(nameCheck);
    printf("------------------ END StateManager_requestToGetState----------------------------\n");
}