void loadNetData() {
  ext_ssid = newReadString(addrExtSSID);
  Serial.println("prendo SSID");
  ext_password = newReadString(addrExtPassword);
  Serial.println("prendo password");
}
void writeString (int address, String data){
  lenght = strlen(data);
  memory_map[address] = lenght;
  for (i = address + 1, a = 0; i < address + lenght + 1; i++, a++){
    memory_map[i] = data[a];
  }
}

String newReadString(int add) {
  char data[100];
  int sec = 0, pos = 0, length;
  unsigned char l;
  for (int sec = 0; sec < add; sec++) {
    pos = pos + EEPROM.read(pos);
  }
  length = EEPROM.read(pos) - 1;
  Serial.print("La lunghezza è: ");
  Serial.println( length);

  pos++;
  for (int i = 0; i < length; i++) {

    data[i] = EEPROM.read(pos + i);
    Serial.print("leggo: ");
    Serial.print( data[i]);
    Serial.print(" in posizione: ");
    Serial.println(i);
  }
  data[length] = '\0';

  Serial.println(String(data));
  return String(data);
}
