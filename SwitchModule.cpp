/**
 *  iotinator Switch Slave module 
 *  Xavier Grosjean 2018
 *  Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International Public License
 */
 
#include "SwitchModule.h"
 
SwitchModule::SwitchModule(SwitchConfigClass* config, int displayAddr, int displaySda, int displayScl):XIOTModule(config, displayAddr, displaySda, displayScl) {
  pinMode(5, OUTPUT);
  _oledDisplay->setLineAlignment(2, TEXT_ALIGN_CENTER);
  setStatus(false);
}

char* SwitchModule::_customData() {
//  Serial.println("SwitchModule::_customData");
  char* data = (char *)malloc(100);
  sprintf(data, "{\"status\":\"%s\"}", _status ? "on": "off");
  return data;  
}

char* SwitchModule::useData(char* data, int* httpCode) {
Serial.println("switch");
Serial.println(data);
  const int bufferSize = 2*JSON_OBJECT_SIZE(1);
  StaticJsonBuffer<bufferSize> jsonBuffer;   
  JsonObject& root = jsonBuffer.parseObject(data); 
  if (!root.success()) {
    _oledDisplay->setLine(1, "Got bad data", TRANSIENT, NOT_BLINKING);
  } else {
    const char *status = (const char*)root["status"];
    if(strcmp(status, "on") == 0) {
      setStatus(true);
    } else {
      setStatus(false);
    }
  }
  *httpCode = 200;
  return emptyMallocedResponse();
}

void SwitchModule::setStatus(bool status) {
  _status = status;
  digitalWrite(5, _status ? HIGH : LOW);
  char message[100];
  sprintf(message, "Switch is %s", _status ? "ON": "OFF");
  _oledDisplay->setLine(2, message, NOT_TRANSIENT, NOT_BLINKING);
}