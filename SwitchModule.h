/**
 *  iotinator Switch module 
 *  Xavier Grosjean 2018
 *  Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International Public License
 */
 
#pragma once 
#include "config.h"
#include <XIOTModule.h>
 
class SwitchModule:public XIOTModule {
public:
  SwitchModule(SwitchConfigClass* config, int displayAddr, int displaySda, int displayScl);
  
protected:
  bool _status = false;  
  char* _customData() override;
  
};