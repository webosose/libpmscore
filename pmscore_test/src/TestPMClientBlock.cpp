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
#include "ClientBlock.h"
#include "PmsConfigLinux.h"
#include "PmsErrors.h"

static GMainLoop *mainLoop = nullptr;

class TestPMClientBlock: public ::testing::Test {
    public:
        TestPMClientBlock()
        {
            printf("TestPMClientBlock constructor entered\n");
            printf("TestPMClientBlock constructor exit\n");
        }

        ~TestPMClientBlock() {
            printf("====TestPMClientBlock destructor entered ===\n");
            printf("====TestPMClientBlock destructor exit ===\n");
        }
};

TEST_F(TestPMClientBlock, ClientBlock_Initialize)
{
    printf("------------------ BEGIN ClientBlock_Initialize ----------------------------\n");
    PmsConfig* pConfig = new PmsConfigLinux("/var/PMS/pms.conf");
    mainLoop = g_main_loop_new(NULL, FALSE);
    ClientBlock *ptrClientBlk = new ClientBlock(pConfig, mainLoop);
    PmsErrorCode_t err = ptrClientBlk->Initialize();
    //! Since no registration done yet
    EXPECT_EQ(err, kPmsErrClientNotRegistered);
    //g_main_loop_run(mainLoop);
    //g_main_loop_unref(mainLoop);
    printf("------------------ END ClientBlock_Initialize----------------------------\n");
}

TEST_F(TestPMClientBlock, ClientBlock_Start)
{
    printf("------------------ BEGIN ClientBlock_Start ----------------------------\n");
    PmsConfig* pConfig = new PmsConfigLinux("/var/PMS/pms.conf");
    mainLoop = g_main_loop_new(NULL, FALSE);
    ClientBlock *ptrClientBlk = new ClientBlock(pConfig, mainLoop);
    PmsErrorCode_t err = ptrClientBlk->Start();
    EXPECT_EQ(err, kPmsSuccess);
    //g_main_loop_run(mainLoop);
    //g_main_loop_unref(mainLoop);
    printf("------------------ END ClientBlock_Start----------------------------\n");
}

TEST_F(TestPMClientBlock, ClientBlock_Deinitialize)
{
    printf("------------------ BEGIN ClientBlock_Deinitialize ----------------------------\n");
    PmsConfig* pConfig = new PmsConfigLinux("/var/PMS/pms.conf");
    mainLoop = g_main_loop_new(NULL, FALSE);
    ClientBlock *ptrClientBlk = new ClientBlock(pConfig, mainLoop);
    PmsErrorCode_t err = ptrClientBlk->Deinitialize();
    EXPECT_EQ(err, kPmsSuccess);
    //g_main_loop_run(mainLoop);
    //g_main_loop_unref(mainLoop);
    printf("------------------ END ClientBlock_Deinitialize----------------------------\n");
}