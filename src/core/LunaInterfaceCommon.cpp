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

#include <assert.h>
#include "LunaInterfaceCommon.h"

/** @name Utility for storing Pms's this pointer for each of the handlers */
//@{
PMSLunaCategoryContext *PMSLunaCategoryContext::pInstance = nullptr;

bool PMSLunaCategoryContext::AddLunaCategoryContext(LunaCategory_t category, LunaContext_t cxt)
{
    assert(mLunaCategoryContext.end() == mLunaCategoryContext.find(category));
    mLunaCategoryContext[category] = cxt;
    return true;
}

LunaContext_t PMSLunaCategoryContext::GetLunaCategoryContext(LunaCategory_t category)
{
    return mLunaCategoryContext[category];
}

PMSLunaCategoryContext* PMSLunaCategoryContext::Instance()
{
    if (pInstance == NULL)
    {
        static PMSLunaCategoryContext obj;
        pInstance = &obj;
    }

    return pInstance;
}
//@}