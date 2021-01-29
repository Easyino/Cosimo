namespace TypeCast = experimental::TypeConversion;
using namespace experimental::crypto;
String encryptString(String data) {
  if (EEPROM.read(1) == chances + 1) {
    interface = 1;
    while (interface == 1) {
      interfaceSelector();
      ESP.wdtFeed();
    }
  }
  String buffer = "";
  ChaCha20Poly1305::encrypt(data.begin(), data.length(), derivedKey, &encryptionCounter, sizeof encryptionCounter, resultingNonce, resultingTag);
  for (i = 0; i < 12; i++) {
    buffer += (char)resultingNonce[i];
  }
  for (i = 0; i < 16; i++) {
    buffer += (char)resultingTag[i];
  }
  buffer += data;
  return buffer;
}
String decryptString(String data) {
  String original_data;
  for (int i = 0; i < 12; i++) {
    resultingNonce[i] = data[i];
  }
  for (int i = 0; i < 16; i++) {
    resultingTag[i] = data[i + 12];
  }
  data.remove(0, 28);
  original_data = data;
retry:
  bool decryptionSucceeded = ChaCha20Poly1305::decrypt(data.begin(), data.length(), derivedKey, &encryptionCounter, sizeof encryptionCounter, resultingNonce, resultingTag);
  if (decryptionSucceeded) {
    if (interface == 0) {
      interface = 2;
      EEPROM.write(1, 0);
      EEPROM.commit();
    }
    return data;
  }
  Serial.println("Decryption failed");
  wrong_key = true;
  interface = 0;
  while (wrong_key) {
    interfaceSelector();
    ESP.wdtFeed();
  }
  data = original_data;
  goto retry;
}
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
