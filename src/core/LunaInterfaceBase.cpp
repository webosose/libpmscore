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

#include "LunaInterfaceBase.h"

CancelSubscriptionTable_t LunaInterfaceBase::mCancelSubscriptionTable;

bool
LunaInterfaceBase::CancelSubscription(LSHandle* pLsH, LSMessage* pLsMsg, void* pData)
{
    MSG_DEBUG("LSHandle = %p LSMessage = %p mCancelSubscriptionTable size = %lu", (void *)pLsH, (void *)pLsMsg, mCancelSubscriptionTable.size());

    bool success = true;
    CancelSubscriptionTable_t::iterator it;
    it = mCancelSubscriptionTable.find(pLsMsg);

    if (it != mCancelSubscriptionTable.end())
    {
        success = it->second(pLsMsg);
        mCancelSubscriptionTable.erase(it);
    }

    MSG_DEBUG("mCancelSubscriptionTable size = %lu", mCancelSubscriptionTable.size());
    return success;
}

bool
LunaInterfaceBase::RegisterCancelSubscriptionCallback(LSMessage* pLsMsg, const CancelSubscriptionFunction_t& callback)
{
    MSG_DEBUG("LSMessage = %p", (void *)pLsMsg);

    CancelSubscriptionFunction_t placeHolder = nullptr;
    std::pair<CancelSubscriptionTable_t::iterator, bool> newItem;
    assert(callback);
    newItem = mCancelSubscriptionTable.insert(CancelSubscriptionTable_t::value_type(pLsMsg, placeHolder));
    if (newItem.second == false)
    {
        return false;
    }

    // We insert a placeholder and then assign it the functor value to
    // avoid a deep copy of the functor object in insert.
    // Ask Vitaly for an explanation if you want to understand more about this optimization.
    try
    {
        newItem.first->second = callback;
    }
    catch (...)
    {
        mCancelSubscriptionTable.erase(newItem.first);
        throw;
    }
    return true;
}
