namespace TypeCast = experimental::TypeConversion;
using namespace experimental::crypto;
String encryptString(String data) {
  if (EEPROM.read(0) == chances + 1) {
    interface = pinInter;
    dialog_interface = 1;
    while (interface == pinInter) {
      interfaceSelector();
      ESP.wdtFeed();
    }
  }
  String buffer = "";
  ChaCha20Poly1305::encrypt(data.begin(), data.length(), derivedKey, &encryptionCounter, sizeof encryptionCounter, resultingNonce, resultingTag);
  for (int i = 0; i < 12; i++) {
    buffer += (char)resultingNonce[i];
  }
  for (int i = 0; i < 16; i++) {
    buffer += (char)resultingTag[i];
  }
  buffer += data;
  return buffer;
}


String decryptString(String data) {
  String original_data = data;
retry:
  for (int i = 0; i < 12; i++) {
    resultingNonce[i] = data[i];
  }
  for (int i = 0; i < 16; i++) {
    resultingTag[i] = data[i + 12];
  }
  data.remove(0, 28);
  bool decryptionSucceeded = false;
  decryptionSucceeded = ChaCha20Poly1305::decrypt(data.begin(), data.length(), derivedKey, &encryptionCounter, sizeof encryptionCounter, resultingNonce, resultingTag);
  if (decryptionSucceeded) {
    if (interface == pinInter) {
      interfaceBack();
      triggButton = -1; //To fix the selection after the pin interface
      element_counter = 0;//To prevent element sovrapposition when we load an encrypted sector in a interface
      EEPROM.write(0, 0);
      EEPROM.commit();
    }
    return data;
  }
  else {
    interface = pinInter;
    dialog_interface = 0;
  }
  Serial.println("Decryption failed");
  wrong_key = true;
  oled_updated = true;// To make it look better I update the screen only when the pin is incorrect
  while (wrong_key) {// Pin interface
    interfaceSelector();
    ESP.wdtFeed();
  }
  data = original_data;
  goto retry;// Retry to decrypt
}


/**
 * @brief Set the Master Key object
 * 
 * @param data the new key
 */
void setMasterKey(String data) {
  for (int i = 0; i < 8; i++) {
    masterKey[i] = ' ';
  }
  for (int i = 0; data[i] != '\0'; i++) {
    masterKey[i] = data[i];
  }
  HKDF hkdfInstance(FPSTR(masterKey), (sizeof masterKey) - 1, hkdfSalt, sizeof hkdfSalt);
  hkdfInstance.produce(derivedKey, sizeof derivedKey);
}
