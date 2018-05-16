/**
 *  Definition of the config data structure for the iotSwitch module and the class to persist it to EEPROM 
 *  Xavier Grosjean 2018
 *  Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International Public License
 */
 
#pragma once

#include <XIOTConfig.h>

#define CONFIG_VERSION 1
#define CONFIG_NAME "iotSwitch"

typedef struct {
  // First two members version and name are mandatory
  unsigned int version;
  char name[NAME_MAX_LENGTH + 1];
   
  char ssid[SSID_MAX_LENGTH + 1];
  char pwd[PWD_MAX_LENGTH + 1];
   
} ConfigDataType;

class ConfigClass:public XEEPROMConfigClass {
public:
  ConfigClass(unsigned int version, const char* name, void* dataPtr);
  void initFromDefault();
  void setSsid(const char* ssid);
  void setPwd(const char* pwd);
  const char* getSsid(void);
  const char* getPwd(void);
protected:
  ConfigDataType* _getConfigPtr(void);  
};
