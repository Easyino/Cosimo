namespace TypeCast = experimental::TypeConversion;
using namespace experimental::crypto;
String encryptString(String data) {
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
  for (int i = 0; i < 12; i++) {
    resultingNonce[i] = data[i];
  }
  for (int i = 0; i < 16; i++) {
    resultingTag[i] = data[i + 12];
  }
  data.remove(0, 28);
  bool decryptionSucceeded = ChaCha20Poly1305::decrypt(data.begin(), data.length(), derivedKey, &encryptionCounter, sizeof encryptionCounter, resultingNonce, resultingTag);
  if (decryptionSucceeded) {
    return data;
  }
  Serial.println("Decryption failed");
  return "Error";
}
void setMasterKey(String data) {
  for (i = 0; i < 8; i++) {
    masterKey[i] = ' ';
  }
  for (i = 0; data[i] != '\0'; i++) {
    masterKey[i] = data[i];
  }
  //getNonceGenerator()(hkdfSalt, sizeof hkdfSalt);
  HKDF hkdfInstance(FPSTR(masterKey), (sizeof masterKey) - 1, hkdfSalt, sizeof hkdfSalt);
  hkdfInstance.produce(derivedKey, sizeof derivedKey);
}
