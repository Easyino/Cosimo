#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "settings.h"
#include <WiFiClient.h>
#include<EEPROM.h>

void setup() {
  Serial.begin(115200);

  ///////////////////////////////
  EEPROM.begin(EEPROM_lenght);
  EEPROM.write(0,2);   
  EEPROM.commit();
  ///////////////////////////////
  
  for (i = 0; i < 500; i++){ ///// 500 da cambiare (solo di prova)
    memory_map[i] = EEPROM.read(i);
    Serial.println(memory_map[i]);
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
