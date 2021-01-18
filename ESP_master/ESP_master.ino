#include "settings.h"
namespace TypeCast = experimental::TypeConversion;
using namespace experimental::crypto;

void setup() {
  pinMode(up, INPUT_PULLUP);
  pinMode(confirm, INPUT_PULLUP);
  pinMode(down, INPUT_PULLUP);
  Serial.begin(230400);
  Serial.println("--------------------------------");

  Wire.begin(SDA_PIN, SCL_PIN, I2C_MASTER);
  display.init();
  display.flipScreenVertically();
  display.setContrast(255);
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  EEPROM.begin(EEPROM_length);
  inputString.reserve(200);

  max_value_address = pow(2, usable_address_bits);

  if (digitalRead(up) == LOW && digitalRead(confirm) == LOW && digitalRead(down) == LOW) {
    eepromClear();
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
  }
  else {
    loadSector(0);
    for (i = 0; i < 16; i++) {
      hkdfSalt[i] = memory_map[0][i];
    }
  }
  HKDF hkdfInstance(FPSTR(masterKey), (sizeof masterKey) - 1, hkdfSalt, sizeof hkdfSalt); // (sizeof masterKey) - 1 removes the terminating null value of the c-string
  hkdfInstance.produce(derivedKey, sizeof derivedKey);
  Serial.println("hdfsalt:");
  for (i = 0; i < 16; i++) {
    Serial.print(hkdfSalt[i]);
    Serial.print(", ");
  }
  Serial.println("");



  if (EEPROM.read(0)) { //check the state of the network, saved in the first byte of EEPROM
    tryConnect();
    
  }
  else {
    createNetwork();
  }

  n_section = 4;
  section[0].title = "nome-0";

  section[1].title = "nome-1";

  section[2].title = "nome-2";

  section[3].title = "nome-3";

  //sendSlave("/*caboom shadow digispark*/", text);
}

void loop() {
  server.handleClient();


  if (digitalRead(up) == LOW && digitalRead(down) == LOW && digitalRead(confirm) == HIGH) {
    OTAupdate();
  }
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(1, 1, String(millis() / 1000));
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  if (ota_initialised){
    display.drawString(128, 52, "OTA  " + wifi_IP);
  }
  else{
    display.drawString(128, 52, wifi_IP);
  }
  display.display();
  if (digitalRead(confirm) == LOW){
    if (checkReady()){
      
    }
    sendSlave("sono vivo", text);
  }

  
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
