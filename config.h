/**
 *  Definition of the config data structure for the iotSwitch module and the class to persist it to EEPROM 
 *  Xavier Grosjean 2018
 *  Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International Public License
 */
 
#pragma once

#include <XIOTConfig.h>
#include <XUtils.h>

#define CONFIG_VERSION 1
#define CONFIG_NAME "iotSwitch"

struct SwitchConfigStruct:XEEPROMConfigDataStruct {
  // First two members version and name are mandatory and inherited from XEEPROMConfigDataStruct
  
  char ssid[SSID_MAX_LENGTH + 1];
  char pwd[PWD_MAX_LENGTH + 1];
   
};

class SwitchConfigClass:public XEEPROMConfigClass {
public:
  SwitchConfigClass(unsigned int version, const char* name);
  void initFromDefault();
  void setSsid(const char* ssid);
  void setPwd(const char* pwd);
  const char* getSsid(void);
  const char* getPwd(void);
protected:
  SwitchConfigStruct* _getDataPtr(void);  
};
