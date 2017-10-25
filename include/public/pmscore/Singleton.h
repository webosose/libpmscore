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

#ifndef PMS_SINGLETON_H
#define PMS_SINGLETON_H

#include <stdio.h>
/**
 *******************************************************************************
 * @brief Singleton class template
 * Use this to make a class into a Singleton.
 * Example:
 * Class C { }; // C can be any class
 * typedef Singleton<C> C_Singleton; //C_Singleton is a Singleton of C
 *
 *******************************************************************************
 */

template <class T>
class Singleton
{
    public:
      /**
       *******************************************************************************
       * @brief Get the singleton instance.
       *
       * @retval - Returns pointer to the singleton instance
       *******************************************************************************
       */
        static T& GetInstance()
        {
           static T instance;

            return instance;
        }

    private:
        Singleton();
        ~Singleton() = delete;
};

#endif /* PMS_SINGLETON_H */
