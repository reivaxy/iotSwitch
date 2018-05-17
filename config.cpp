/**
 *  Class to persist the iotSwitch module configuration data structure to EEPROM 
 *  Xavier Grosjean 2018
 *  Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International Public License
 */
 
#include "config.h"


SwitchConfigClass::SwitchConfigClass(unsigned int version, const char* name):XEEPROMConfigClass(version, name, sizeof(SwitchConfigStruct)) {
}

/**
 * Reset the config data structure to the default values.
 * This is done each time the data structure version is different from the one saved in EEPROM
 * NB: version and name are handled by base class 
 */
void SwitchConfigClass::initFromDefault() {
  XEEPROMConfigClass::initFromDefault(); // handles version and name init 
  SwitchConfigStruct* configPtr = _getDataPtr();
  setSsid(DEFAULT_APSSID);
  setPwd(DEFAULT_APPWD);

}

void SwitchConfigClass::setSsid(const char* ssid) {
  XUtils::safeStringCopy(_getDataPtr()->ssid, ssid, SSID_MAX_LENGTH);
}

void SwitchConfigClass::setPwd(const char* pwd) {
  XUtils::safeStringCopy(_getDataPtr()->pwd, pwd, PWD_MAX_LENGTH);
}

const char* SwitchConfigClass::getSsid() {
  return _getDataPtr()->ssid;
}

const char* SwitchConfigClass::getPwd() {
  return _getDataPtr()->pwd;
}

/**
 * Return the typed data structure object
 *
 */
SwitchConfigStruct* SwitchConfigClass::_getDataPtr(void) {
  return (SwitchConfigStruct*)XEEPROMConfigClass::_getDataPtr();
}