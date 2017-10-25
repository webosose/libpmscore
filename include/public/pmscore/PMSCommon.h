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

#ifndef PMSCOMMON_H
#define PMSCOMMON_H

#include <cstddef>
#include <new>

// ensure that we always use the nothrow version of new internally
inline void *operator new(std::size_t size)
{
    return operator new(size, std::nothrow);
}

inline void *operator new[](std::size_t size)
{
    return operator new[](size, std::nothrow);
}

#endif /* PMSCOMMON_H */

