#include "settings.h"
namespace TypeCast = experimental::TypeConversion;
using namespace experimental::crypto;

void setup() {
  pinMode(button_up, INPUT_PULLUP);
  pinMode(button_confirm, INPUT_PULLUP);
  pinMode(button_down, INPUT_PULLUP);
  Serial.begin(230400);
  Serial.println("--------------------------------");

  Wire.begin(D1, D2, I2C_MASTER);
  display.init();
  display.flipScreenVertically();
  display.setContrast(255);
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  EEPROM.begin(EEPROM_length);
  inputString.reserve(200);
  max_value_address = pow(2, usable_address_bits);

  if (!digitalRead(button_up) && !digitalRead(button_confirm) && !digitalRead(button_down)) {
    eepromClear();
    while (!digitalRead(button_up) && !digitalRead(button_confirm) && !digitalRead(button_down));
  }


  Serial.print("max address memory = ");
  Serial.println(max_value_address);
  loadCheckpoints();
  if (checkpoint_memory[0] != 17) {
    eepromClear();
    loadSector(0);
    getNonceGenerator()(hkdfSalt, sizeof hkdfSalt);
    String buffer;
    for (i = 0; i < 16; i++) {
      buffer += (char)hkdfSalt[i];
    }
    updateCommand(0, buffer, text);
    updateEEPROM();
    EEPROM.write(1, chances + 1);
    EEPROM.commit();
  }
  else {
    loadSector(0);
    for (i = 0; i < 16; i++) {
      hkdfSalt[i] = memory_map[0][i];
    }
  }
  HKDF hkdfInstance(FPSTR(masterKey), (sizeof masterKey) - 1, hkdfSalt, sizeof hkdfSalt); // (sizeof masterKey) - 1 removes the terminating null value of the c-string
  hkdfInstance.produce(derivedKey, sizeof derivedKey);
  if (EEPROM.read(0)) { //check the state of the network, saved in the first byte of EEPROM
    tryConnect();
  }
  else {
    createNetwork();
  }


  ///////////////////////////////////////////////////////Demo code to try thigns
  n_section = 4;
  section[0].title = "nome-0";
  section[1].title = "nome-1";
  section[2].title = "nome-2";
  section[3].title = "nome-3";
}

void loop() {

  server.handleClient();

  if (!digitalRead(button_up) && !digitalRead(button_down) && digitalRead(button_confirm)) {
    OTAupdate();
  }

  if (ota_initialised) {
    ArduinoOTA.handle();
  }

  interfaceSelector();

  serialEvent();
  if (stringComplete) {
    stringComplete = false;
    loadSerialCommands(inputString);
    executeSerialCommands();
  }
}
