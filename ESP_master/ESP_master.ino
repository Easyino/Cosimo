#include "settings.h"
namespace TypeCast = experimental::TypeConversion;
using namespace experimental::crypto;

void setup() {
  pinMode(button_up, INPUT_PULLUP);
  pinMode(button_confirm, INPUT_PULLUP);
  pinMode(button_down, INPUT_PULLUP);
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println("--------------------------------");

  //Wire.begin(D1, D2, I2C_MASTER);
  EEPROM.begin(EEPROM_length);
  inputString.reserve(200);
  display.init();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  eepromPar(2);
  eepromPar(3);
  eepromPar(4);
  
  max_value_address = pow(2, usable_address_bits);

  if (!digitalRead(button_up) && !digitalRead(button_confirm) && !digitalRead(button_down)) {// Press the 3 buttons to erase EEPROM
    eepromClear();
    while (!digitalRead(button_up) && !digitalRead(button_confirm) && !digitalRead(button_down));
  }


  Serial.print("max address memory = ");
  Serial.println(max_value_address);
  loadCheckpoints();
  if (checkpoint_memory[1] != 20 + EEPROM_offset) {// Detect resetting
    Serial.println("------------First set------------");
    eepromClear();
    loadSector(0);
    getNonceGenerator()(hkdfSalt, sizeof hkdfSalt);
    String buffer;
    for (i = 0; i < 16; i++) {
      buffer += (char)hkdfSalt[i];
    }
    updateCommand(0, buffer, text);// Saving salt
    updateEEPROM();
    EEPROM.write(0, chances + 1);// Used when it has to be decided the key
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


  //ESP.eraseConfig();  
  //WiFi.persistent(false);
  //wifi_set_phy_mode(PHY_MODE_11B);
  //eepromPar(1);
  //tryConnect();
  createNetwork();

  ///////////////////////////////////////////////////////Demo code to try thigns
  n_section = 4;
  section[0].title = "nome-0";
  section[1].title = "nome-1";
  section[2].title = "nome-2";
  section[3].title = "nome-3";
}

void loop() {
  unsigned int s = WiFi.status();
  if (status != s) { // WLAN status change
      Serial.print("Status: ");
      Serial.println(s);
      status = s;
      if (s == WL_CONNECTED) {
        // Setup MDNS responder
        if (!MDNS.begin(myHostname)) {
          Serial.println("Error setting up MDNS responder!");
        } else {
          Serial.println("mDNS responder started");
          // Add service to MDNS-SD
          MDNS.addService("http", "tcp", 80);
        }
      } else if (s == WL_NO_SSID_AVAIL) {
        WiFi.disconnect();
      }
    }
    if (s == WL_CONNECTED) {
      MDNS.update();
    }
  dnsServer.processNextRequest();
  server.handleClient();

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