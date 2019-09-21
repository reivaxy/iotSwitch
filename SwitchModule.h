/**
 *  iotinator Switch module 
 *  Xavier Grosjean 2018
 *  Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International Public License
 */
 
#pragma once 
#include "config.h"
#include <XIOTModule.h>
#include "onOffFont.h"
 
class SwitchModule:public XIOTModule {
public:
  SwitchModule(SwitchConfigClass* config, int displayAddr, int displaySda, int displayScl, int relayPin);
  
protected:
  bool _status = true;  
  char* _customData() override;
  char* _globalStatus() override;
  char* useData(char *, int *) override;
  bool customProcessSMS(const char* phoneNumber, const bool isAdmin, const char* message) override;
  void setStatus(bool);
  int _relayPin;
};