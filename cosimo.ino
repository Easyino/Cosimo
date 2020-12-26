#include <EEPROM.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include "settings.h"
#include <WiFiClient.h>


void setup() {
  Serial.begin(115200);
  EEPROM.begin(EEPROM_lenght);
  inputString.reserve(200);
  //eepromClear();
  loadCheckpoints();
  loadSector(0);
  for (i = 0; i < settings_bytes; i++) {
    settings[i] = memory_map[i][0];
  }
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
