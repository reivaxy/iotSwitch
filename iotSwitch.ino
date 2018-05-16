/**
 *  iotinator Switch Slave module 
 *  Xavier Grosjean 2018
 *  Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International Public License
 */
 
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <stdio.h>
#include <TimeLib.h>
#include <XIOTDisplay.h>

#include "config.h"

#define API_VERSION "1.0"    // modules can check API version to make sure they are compatible...

// Global object to store config
ConfigDataType switchConfigData;
ConfigClass *config;
DisplayClass *oledDisplay;

// I couldn't find a way to instanciate this in the XOLEDDisplay lib
// and keep it working further than in the constructor...
SSD1306 display(0x3C, D5, D6);

ESP8266WebServer server(80);
time_t timeNow = 0; 
time_t timeLast = 0;
time_t timeLastGet = 0;
bool wifiConnected = false;
bool canGet = false;

char *localIP;

// Handlers will work as long as these variables exists. 
static WiFiEventHandler wifiSTAGotIpHandler, wifiSTADisconnectedHandler;
bool defaultAP = true;

void setup(){
  Serial.begin(9600);
  delay(100);
  
  config = new ConfigClass((unsigned int)CONFIG_VERSION, (char*)CONFIG_NAME, (void*)&switchConfigData);
  config->init();
  Serial.println(config->getName());
 
  // Module is Wifi Station only
  WiFi.mode(WIFI_STA);
  // Initialise the OLED display
  oledDisplay = new DisplayClass(&display);
  initDisplay();

  // After a reset, open Default Access Point
  // If Access Point was customized, we'll switch to it after one minute
  // This is supposed to give slave modules time to initialize.
  connectSTA();
  wifiSTAGotIpHandler = WiFi.onStationModeGotIP(onSTAGotIP); 
  wifiSTADisconnectedHandler = WiFi.onStationModeDisconnected(onSTADisconnected);      
}

/*********************************
 * Main Loop
 *********************************/
void loop() {
  // Check if any request to serve
  //server.handleClient();
 
  // Display needs to be refreshed periodically to handle blinking
  oledDisplay->refresh();

  timeNow = now();
  if(timeNow - timeLast >= 1) {
    timeLast = timeNow;
    timeDisplay();
  }
  
  if(canGet) {
    timeLastGet = timeNow;
    canGet = false;
    // Get the config information from the master
    Serial.println("get /api/config");
    HTTPClient http;
    
    // TODO : get IP from somewhere...
    http.begin("http://192.168.4.1/api/config");
    int httpCode = http.GET();   // Log stuff to Serial ?
    Serial.print("HTTP code: ");
    Serial.println(httpCode);
    Serial.printf("HTTP GET failed, error: %s\n", http.errorToString(httpCode).c_str());
    Serial.println(http.getString());
      
    http.end();    
  }    
  delay(20);
  
}

// Called when STA is connected to home wifi and IP was obtained
void onSTAGotIP (WiFiEventStationModeGotIP ipInfo) {
  XUtils::stringToCharP(ipInfo.ip.toString(), &localIP);
  Serial.printf("Got IP on %s: %s\n", config->getSsid(), localIP);
  wifiConnected = true;
  canGet = true;
  wifiDisplay();

}

void onSTADisconnected(WiFiEventStationModeDisconnected event) {
  // Continuously get messages, so just output once.
  if(wifiConnected) {
    free(localIP);
    Serial.printf("Lost connection to %s, error: %d\n", event.ssid.c_str(), event.reason);
    wifiConnected = false;
    wifiDisplay();
  }
}

void connectSTA() {
  WiFi.begin(config->getSsid(), config->getPwd());
  wifiDisplay();
}

void initDisplay( void ) {
  oledDisplay->setTitle(config->getName());
  wifiDisplay();
  timeDisplay();
}

void timeDisplay() {
}

void wifiDisplay() {
  char message[100];
  WifiType wifiType = STA;
  bool blinkWifi = false;

  if(wifiConnected) {
    strcpy(message, config->getSsid());
    strcat(message, " ");
    strcat(message, localIP);
  } else {
    message[0] = 0;
  }
  oledDisplay->setLine(0, message);
  
  if (!wifiConnected) {
    blinkWifi = true;
  }
  oledDisplay->wifiIcon(blinkWifi, wifiType);
}