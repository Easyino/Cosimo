#include <EEPROM.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include "settings.h"
#include <WiFiClient.h>
#include <Wire.h>
#include <PolledTimeout.h>
#include <TypeConversion.h>
#include <Crypto.h>
namespace TypeCast = experimental::TypeConversion;
using namespace experimental::crypto;
uint8_t resultArray[SHA256::NATURAL_LENGTH] { 0 };
uint8_t derivedKey[ENCRYPTION_KEY_LENGTH] { 0 };



void setup() {
  pinMode(up, INPUT_PULLUP);
  pinMode(confirm, INPUT_PULLUP);
  pinMode(down, INPUT_PULLUP);
  //Wire.begin(SDA_PIN, SCL_PIN, I2C_MASTER);


  Serial.begin(2000000);
  Serial.println("--------------------------------");

  EEPROM.begin(EEPROM_length);
  inputString.reserve(200);
  max_value_address = pow(2, usable_address_bits);
  Serial.print("max address memory = ");
  Serial.println(max_value_address);
  loadCheckpoints();
  loadSector(0);
  for (i = 0; i < settings_bytes; i++) {
    settings[i] = memory_map[i][0];
  }

  if (!tryConnect())createNetwork();
}

void loop() {
  if (netStat) {
    server.handleClient();
  }
  serialEvent();
  if (stringComplete) {
    stringComplete = false;
    loadSerialCommands(inputString);
    executeSerialCommands();
  }
}
