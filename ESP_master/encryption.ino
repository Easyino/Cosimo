namespace TypeCast = experimental::TypeConversion;
using namespace experimental::crypto;
String encryptString(String data) {
  if (EEPROM.read(1) == chances + 1) {
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
  bool decryptionSucceeded = ChaCha20Poly1305::decrypt(data.begin(), data.length(), derivedKey, &encryptionCounter, sizeof encryptionCounter, resultingNonce, resultingTag);
  if (decryptionSucceeded) {
    if (interface == pinInter) {
      interface = previous_interface;
      EEPROM.write(1, 0);
      EEPROM.commit();
    }
    return data;
  }
  Serial.println("Decryption failed");
  wrong_key = true;
  interface = pinInter;
  dialog_interface = 0;
  oled_updated = true;// This is an exceptional declaration to make the pin interface work better
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
  for (i = 0; i < 8; i++) {
    masterKey[i] = ' ';
  }
  for (i = 0; data[i] != '\0'; i++) {
    masterKey[i] = data[i];
  }
  HKDF hkdfInstance(FPSTR(masterKey), (sizeof masterKey) - 1, hkdfSalt, sizeof hkdfSalt);
  hkdfInstance.produce(derivedKey, sizeof derivedKey);
}
