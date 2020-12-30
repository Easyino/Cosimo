#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#include <ArduinoOTA.h>

#include <EEPROM.h>
#include <Wire.h>
#include <PolledTimeout.h>
#include <TypeConversion.h>
#include <Crypto.h>

#include "settings.h"

namespace TypeCast = experimental::TypeConversion;
using namespace experimental::crypto;
uint8_t resultArray[SHA256::NATURAL_LENGTH] { 0 };
uint8_t derivedKey[ENCRYPTION_KEY_LENGTH] { 0 };
#include "SSD1306Wire.h"
#include "images.h"
SSD1306Wire display(0x3c, SDA, SCL);


void setup() {
  pinMode(up, INPUT_PULLUP);
  pinMode(confirm, INPUT_PULLUP);
  pinMode(down, INPUT_PULLUP);
  Serial.begin(250000);
  Serial.println("--------------------------------");


  //Wire.begin(SDA_PIN, SCL_PIN, I2C_MASTER);
  display.init();
  display.flipScreenVertically();
  display.setContrast(255);


  EEPROM.begin(EEPROM_length);
  inputString.reserve(200);
  max_value_address = pow(2, usable_address_bits);

  if (digitalRead(D5) == LOW && digitalRead(D6) == LOW && digitalRead(D7) == LOW) {
    eepromClear();
  }


  Serial.print("max address memory = ");
  Serial.println(max_value_address);
  loadCheckpoints();
  loadSector(0);
  for (i = 0; memory_map[i] != ""; i++) {
    settings[i] = memory_map[i][0];
  }

  if (EEPROM.read(0)) {
    tryConnect();
  }
  else {
    createNetwork();
  }


  
  if (digitalRead(D5) == LOW && digitalRead(D7) == LOW) {
    OTAupdate();
  }

  n_section = 4;
  section[0].title = "nome-0";
  section[0].email = "email-0";
  section[0].password = "password-0";
  section[1].title = "nome-1";
  section[1].email = "email-1";
  section[1].password = "password-1";
  section[2].title = "nome-2";
  section[2].email = "email-2";
  section[2].password = "password-2";
  section[3].title = "nome-3";
  section[3].email = "email-3";
  section[3].password = "password-3";
  
}

void loop() {
    server.handleClient();
  

  if (ota_initialised) {
    ArduinoOTA.handle();
  }

  serialEvent();
  if (stringComplete) {
    stringComplete = false;
    loadSerialCommands(inputString);
    executeSerialCommands();
  }
}
