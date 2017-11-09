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

#include <map>

//@{
/** This utility is for storing the this pointer for all the
    Luna Classes and would be retrieved from the callback
    functions. This is a singleton class.
 */
#include "LunaInterfaceBase.h"

#define kPmsMsgCategoryMask           0x0FF00000
#define kPmsMsgCategoryOffset         0x00100000

#define kPmsMsgCategoryRoot          (kPmsMsgCategoryOffset * 1)

typedef unsigned int LunaCategory_t;    /** category */

typedef LunaInterfaceBase* LunaContext_t;     /** this pointer */

class PMSLunaCategoryContext
{
public:
    static PMSLunaCategoryContext* Instance();

    bool AddLunaCategoryContext( LunaCategory_t category, LunaContext_t context );
    LunaContext_t GetLunaCategoryContext( LunaCategory_t category );

protected:
    PMSLunaCategoryContext()
    {
        mLunaCategoryContext.clear( );
    }

private:
    static PMSLunaCategoryContext* pInstance;

    std::map<LunaCategory_t, LunaContext_t> mLunaCategoryContext;
};