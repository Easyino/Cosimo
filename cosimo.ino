#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "settings.h"
#include <WiFiClient.h>
#include<EEPROM.h>

void setup() {
  Serial.begin(115200);
  EEPROM.begin(EEPROM_lenght); 
  //eepromClear();
  loadCheckpoints();
  settings = loadSector(0);
  settings_lenght = command_lenght;
  Serial.println("sketch di prova etc etc");
  max_value_address = exponential(2, usable_address_bits);
  Serial.print("max address memory = ");
  Serial.println(max_value_address);
  if (!tryConnect())createNetwork();
}

void loop() {
  if (netStat) {
    server.handleClient();
  }

}
