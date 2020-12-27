String encryptString(String data){
  using namespace experimental::crypto;
  ChaCha20Poly1305::encrypt(data.begin(), data.length(), derivedKey, &encryptionCounter, sizeof encryptionCounter, resultingNonce, resultingTag);
  return data;
}
String decryptString(String data){
  using namespace experimental::crypto;
  bool decrypted = ChaCha20Poly1305::decrypt(data.begin(), data.length(), derivedKey, &encryptionCounter, sizeof encryptionCounter, resultingNonce, resultingTag);
  if (decrypted){
    return data;
  }
  Serial.println("Decryption failed");
  return "Error";
}
