/**
 *  Class to persist the iotSwitch module configuration data structure to EEPROM 
 *  Xavier Grosjean 2018
 *  Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International Public License
 */
 
#include "config.h"


SwitchConfigClass::SwitchConfigClass(unsigned int version, const char* name):ModuleConfigClass(version, "iotswitch", name, sizeof(SwitchConfigStruct)) {
}

/**
 * Reset the config data structure to the default values.
 * This is done each time the data structure version is different from the one saved in EEPROM
 * NB: version and name are handled by base class 
 */
void SwitchConfigClass::initFromDefault() {
  ModuleConfigClass::initFromDefault(); // handles version and name init, ssid and pwd

}

const char* SwitchConfigClass::getDefaultUIClassName() {
  Serial.println("SwitchConfigClass::getDefaultUIClassName");
  return "switchUIClass";
}

/**
 * Return the typed data structure object
 *
 */
SwitchConfigStruct* SwitchConfigClass::_getDataPtr(void) {
  return (SwitchConfigStruct*)ModuleConfigClass::_getDataPtr();
}