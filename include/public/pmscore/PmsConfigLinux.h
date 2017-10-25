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

#ifndef PMS_CONFIG_LINUX_H
#define PMS_CONFIG_LINUX_H

#include "PmsConfig.h"

class PmsConfigLinux: public PmsConfig
{

public:
  virtual PmsErrorCode_t GetString(const std::string& category, const std::string& key,
      std::string* value);
  virtual PmsErrorCode_t GetStringList(const std::string& category, const std::string& key,
      std::vector<std::string>* value);

  /**
   * The constructor will load the TIL config file and pupulate the mKey member
   * variable which will be used by the functions to read the config file.
   */

  PmsConfigLinux(const std::string pathToFile);
  virtual ~PmsConfigLinux();

  protected:

  private:
   PmsConfigLinux();
   GKeyFile* mpGkey_;
};

#endif
