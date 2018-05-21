/**
 *  iotinator Switch Slave module 
 *  Xavier Grosjean 2018
 *  Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International Public License
 */
 
#include "SwitchModule.h"
 
SwitchModule::SwitchModule(SwitchConfigClass* config, int displayAddr, int displaySda, int displayScl):XIOTModule(config, displayAddr, displaySda, displayScl) {
  pinMode(5, OUTPUT);
}

char* SwitchModule::_customData() {
//  Serial.println("SwitchModule::_customData");
  char* data = (char *)malloc(100);
  sprintf(data, "{\"status\":\"%s\"}", _status ? "on": "off");
  digitalWrite(5, _status ? HIGH : LOW);
  _status = !_status;
  
  
  return data;  
}