/**
 *  Definition of the config data structure for the iotSwitch module and the class to persist it to EEPROM 
 *  Xavier Grosjean 2018
 *  Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International Public License
 */
 
#pragma once

#include <XIOTConfig.h>
#include <XUtils.h>

#define CONFIG_VERSION 1
#define MODULE_NAME "Switch"

struct SwitchConfigStruct:ModuleConfigStruct {
  // Add config fields needed if any
};

class SwitchConfigClass:public ModuleConfigClass {
public:
  SwitchConfigClass(unsigned int version, const char* name);
  void initFromDefault();
  const char* getDefaultUIClassName() override;

protected:
  SwitchConfigStruct* _getDataPtr(void);  
};
