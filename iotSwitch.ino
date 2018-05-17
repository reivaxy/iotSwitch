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
#include <XIOTModule.h>

#include "config.h"

#define API_VERSION "1.0"    // modules can check API version to make sure they are compatible...

// Global object to store config
SwitchConfigClass *config;
DisplayClass *oledDisplay;

ESP8266WebServer server(80);
time_t timeNow = 0; 
time_t timeLast = 0;
time_t timeLastGet = 0;
bool wifiConnected = false;
bool canGet = false;
bool timeInitialized = false;

char *localIP;

// Handlers will work as long as these variables exists. 
static WiFiEventHandler wifiSTAGotIpHandler, wifiSTADisconnectedHandler;
bool defaultAP = true;

void setup(){
  Serial.begin(9600);
  delay(100);
  
  config = new SwitchConfigClass((unsigned int)CONFIG_VERSION, (char*)CONFIG_NAME);
  config->init();
  Serial.println(config->getName());
 
  // Module is Wifi Station only
  WiFi.mode(WIFI_STA);
  // Initialise the OLED display
  oledDisplay = new DisplayClass(0x3C, D5, D6);
  initDisplay();

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
  
  // Should we get the config from master ?
  if(wifiConnected && canGet) {
    timeLastGet = timeNow;
    canGet = false;
    getConfigFromMaster();
  }    
  delay(20);
  
}

void getConfigFromMaster() {
  // Get the config information from the master
  Serial.println("get /api/config");
  HTTPClient http;
  
  // TODO : get IP from somewhere...
  http.begin("http://192.168.4.1/api/config");
  int httpCode = http.GET();   // Log stuff to Serial ?
  Serial.print("HTTP code: ");
  Serial.println(httpCode);
  if(httpCode <= 0) {
    Serial.printf("HTTP GET failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
  String jsonConfig = http.getString();
  http.end();

  Serial.println(jsonConfig);

  StaticJsonBuffer<CONFIG_PAYLOAD_SIZE> jsonBuffer; 
  JsonObject& root = jsonBuffer.parseObject(jsonConfig);
  bool masterTimeInitialized = root[XIOTModuleJsonTag::timeInitialized];
  if(masterTimeInitialized) {
    long timestamp = root[XIOTModuleJsonTag::timestamp];
    setTime(timestamp);
    timeInitialized = true;
  }
  bool APInitialized = root[XIOTModuleJsonTag::APInitialized];
  // If access point on Master was customized, get its ssid and password, 
  // Save them in EEProm
  if(APInitialized) {
    const char *ssid = root[XIOTModuleJsonTag::APSsid];   
    const char *pwd = root[XIOTModuleJsonTag::APPwd];
    if(strcmp(pwd, config->getPwd()) != 0) {
      config->setSsid(ssid);
      config->setPwd(pwd);
      config->saveToEeprom();
      connectSTA();
    }
  }
    
}

// Called when STA is connected to home wifi and IP was obtained
void onSTAGotIP (WiFiEventStationModeGotIP ipInfo) {
  XUtils::stringToCharP(ipInfo.ip.toString(), &localIP);
  Serial.printf("Got IP on %s: %s\n", config->getSsid(), localIP);
  wifiConnected = true;
  canGet = true;  // Can't perform an http get from within the handler, it fails...
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
  oledDisplay->clockIcon(!timeInitialized);
  if(timeInitialized) {
    int h = hour();
    int mi = minute();
    int s = second();
    int d = day();
    int mo = month();
    int y= year();
    char message[30];
    sprintf(message, "%02d:%02d:%02d %02d/%02d/%04d", h, mi, s, d, mo, y);
    oledDisplay->refreshDateTime(message);
  }
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
    sprintf(message, "Connecting to %s", config->getSsid());
  }
  oledDisplay->setLine(0, message);
  
  if (!wifiConnected) {
    blinkWifi = true;
  }
  oledDisplay->wifiIcon(blinkWifi, wifiType);
}