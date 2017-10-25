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

#ifndef PMS_CONFIG_H
#define PMS_CONFIG_H

#include <string>
#include <vector>
#include "log.h"
#include "PmsErrors.h"


class PmsConfig
{
public:
  virtual ~PmsConfig()
  {
    delete mpLog;
    mpLog = nullptr;
  }

  virtual PmsErrorCode_t GetString(const std::string& category, const std::string& key,
      std::string* value) = 0;
  virtual PmsErrorCode_t GetStringList(const std::string& category, const std::string& key,
      std::vector<std::string>* value) = 0;

protected:
  PmsConfig(const std::string pathToFile): pathToPmsConf_( pathToFile) { };

  //This is the string path to the pms config file
  const std::string pathToPmsConf_;
  const Logger* mpLog;
};

#endif
