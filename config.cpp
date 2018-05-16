/**
 *  Class to persist the iotSwitch module configuration data structure to EEPROM 
 *  Xavier Grosjean 2018
 *  Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International Public License
 */
 
#include "config.h"


ConfigClass::ConfigClass(unsigned int version, const char* name, void* dataPtr):XEEPROMConfigClass(version, name, dataPtr, sizeof(ConfigDataType)) {
}

/**
 * Reset the config data structure to the default values.
 * This is done each time the data structure version is different from the one saved in EEPROM
 * NB: version and name are handled by base class 
 */
void ConfigClass::initFromDefault() {
  XEEPROMConfigClass::initFromDefault(); // handles version and name init 
  ConfigDataType* configPtr = _getConfigPtr();
  setSsid(DEFAULT_APSSID);
  setPwd(DEFAULT_APPWD);

}

void ConfigClass::setSsid(const char* ssid) {
  XUtils::safeStringCopy(_getConfigPtr()->ssid, ssid, SSID_MAX_LENGTH);
}

void ConfigClass::setPwd(const char* pwd) {
  XUtils::safeStringCopy(_getConfigPtr()->pwd, pwd, PWD_MAX_LENGTH);
}

const char* ConfigClass::getSsid() {
  return _getConfigPtr()->ssid;
}

const char* ConfigClass::getPwd() {
  return _getConfigPtr()->pwd;
}
/**
 * Return the typed data structure object
 *
 */
ConfigDataType* ConfigClass::_getConfigPtr(void) {
  return (ConfigDataType*)getData();
}