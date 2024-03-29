/**
 *  iotinator Switch Agent module
 *  Xavier Grosjean 2018
 *  Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International Public License
 */
 

#include <stdio.h>
#include "SwitchModule.h"

#include "config.h"

#define API_VERSION "1.0"    // modules can check API version to make sure they are compatible...

// Global object to store config
SwitchConfigClass *config;
XIOTModule* module;

int scl = 12;
int sda = 14;
int relayPin = 5;
int inPin = 4;


void setup(){
  // ESP01 is defined through compilation directive when compiling for esp01 board
  // It's defined in platformio.ini file for the esp01 environment.
  #ifdef ESP01
  Serial.begin(9600,SERIAL_8N1,SERIAL_TX_ONLY); 
  scl = 2;
  sda = 0;
  relayPin = 3;
  inPin = 1;
  #else
  Serial.begin(9600);
  #endif
  delay(500); // time to connect serial
  
  config = new SwitchConfigClass((unsigned int)CONFIG_VERSION, (char*)MODULE_NAME);
  config->init();
 
  module = new SwitchModule(config, 0x3C, sda, scl, relayPin, inPin);
     
}

/*********************************
 * Main Loop
 *********************************/
void loop() {
  // Mandatory periodic call, to refresh display, Clock, check requests
  // on the http API, ...
  module->loop();
}

