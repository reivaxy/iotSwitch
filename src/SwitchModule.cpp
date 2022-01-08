/**
 *  iotinator Switch Agent module
 *  Xavier Grosjean 2018
 *  Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International Public License
 */
 
#include "SwitchModule.h"
 
 #define LONG_PUSH_DURATION 2000
  
SwitchModule::SwitchModule(SwitchConfigClass* config, int displayAddr, int displaySda, int displayScl,
                            int relayPin, int inPin):XIOTModule(config, displayAddr, displaySda, displayScl, true, 200) {
  _relayPin = relayPin;
  pinMode(relayPin, OUTPUT);
  _inPin = inPin;
  pinMode(_inPin, INPUT);

  _oledDisplay->setLinePosition(2, 0, 32);
  _oledDisplay->setLineFont(2, onOffFont);
  _oledDisplay->setLineAlignment(2, TEXT_ALIGN_CENTER);
  setStatus(false);
}

char* SwitchModule::_customData() {
//  Serial.println("SwitchModule::_customData");
  char* data = (char *)malloc(100);
  sprintf(data, "{\"status\":\"%s\"}", _status ? "on": "off");
  return data;  
}

char* SwitchModule::useData(const char* data, int* httpCode) {
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

bool SwitchModule::customProcessSMS(const char* phoneNumber, const bool isAdmin, const char* message) {
  if(strcmp(message, "on") * strcmp(message, "1") == 0) {
    setStatus(true);
  } else {
    if(strcmp(message, "off") * strcmp(message, "0") == 0) {
      setStatus(false);
    }
  }
  return true;
}

char* SwitchModule::_globalStatus() {
  return strdup(_status?"on":"off");  
}

void SwitchModule::setStatus(bool status) {
  _status = status;
  digitalWrite(_relayPin, _status ? HIGH : LOW);
  char message[100];
  sprintf(message, "%s", _status ? "AB": "ACC");     // ON or OFF in the specific font used
  _oledDisplay->setLine(2, message, NOT_TRANSIENT, NOT_BLINKING);
}

void SwitchModule::customLoop() {
   int level = digitalRead(_inPin);
   if (level == HIGH) {
     _oledDisplay->setLine(3, "push", TRANSIENT, NOT_BLINKING);
     // start time counter
     if (_beginInputHigh == 0) {
      _beginInputHigh = millis();
     }
     // Is button pushed longer than LONG_PUSH_DURATION ?       
     if (XUtils::isElapsedDelay(millis(), &_beginInputHigh, LONG_PUSH_DURATION)) {
       ESP.restart(); 
     }
   } else {
     _oledDisplay->setLine(3, "", TRANSIENT, NOT_BLINKING);
     // Short push : toggle status
     if (_beginInputHigh != 0) {
       _beginInputHigh = 0;
       setStatus(!_status);
       sendData(false);
     }
   }
     
}