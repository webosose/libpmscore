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

#include "PmsConfigLinux.h"

PmsConfigLinux::PmsConfigLinux(const std::string pathToFile): PmsConfig(pathToFile)
{
    std::string errMsg("Unknown internal error");
    GError* pGerr = nullptr;

    mpGkey_ = g_key_file_new();

    if (g_key_file_load_from_file(mpGkey_, pathToFile.c_str(), G_KEY_FILE_NONE, &pGerr) == FALSE)
    {
        MSG_WARNING("Key file %s load error\n", pathToFile.c_str());
        if (pGerr)
        {
            errMsg = pGerr->message;
            g_error_free(pGerr);
            pGerr = nullptr;
        }
    }
}

PmsConfigLinux::~PmsConfigLinux()
{
    // release the memory allocated by g_key_file_new();
    g_key_file_free(mpGkey_);
}


PmsErrorCode_t PmsConfigLinux::GetString(const std::string& category, const std::string& key,
                                         std::string* value)
{
    PmsErrorCode_t err = kPmsSuccess;
    GError* pGerr = nullptr;
    gchar*  keyValue = nullptr;

    if ((keyValue = g_key_file_get_string(mpGkey_,
                                          category.c_str(),
                                          key.c_str(),
                                          &pGerr)) == NULL)
    {
        err = kPmsErrUnknown;

        if (pGerr)
        {
            g_error_free(pGerr);
        }
    }
    else
    {
        *value = g_strstrip(keyValue);
        g_free(keyValue);
    }

    return err;
}

PmsErrorCode_t PmsConfigLinux::GetStringList(const std::string& category, const std::string& key,
                                             std::vector<std::string>* value)
{
    PmsErrorCode_t err = kPmsSuccess;
    GError* pGerr = nullptr;
    gchar** keyValue = nullptr;
    gsize   numKey = 0;

    if ((keyValue = g_key_file_get_string_list(mpGkey_,
                                               category.c_str(),
                                               key.c_str(),
                                               &numKey,
                                               &pGerr)) == NULL)
    {
        err = kPmsErrUnknown;

        if (pGerr)
        {
            g_error_free(pGerr);
        }
    }
    else
    {
        for(uint8_t i = 0; i < numKey; i++)
        {
            value->push_back(g_strstrip(keyValue[i])) ;
        }

        g_strfreev(keyValue);
    }

    return err;
}
