#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "settings.h"
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include<EEPROM.h>

void setup() {
Serial.begin(115200);
EEPROM.begin(4096);
  Serial.println("sketch di prova etc etc");

if(!tryConnect())createNetwork();

}

void loop() {
  
  if(netStat){
  server.handleClient();
  }
}
