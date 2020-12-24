void loadNetData() {
  ext_ssid = readString(addrExtSSID);
  Serial.println("prendo SSID");
  ext_password = readString(addrExtPassword);
  Serial.println("prendo password");
}

void writeString (int address, String data){
  for(lenght = 0; data[lenght] > 0; lenght++){}
  
  for (i = lenght, a = 0; i > 0; i -= 256, a++){
    if (i > 255){
      memory_map[address + 256 * a] = 255;
    }
    else {
      memory_map[address + 256 * a] = i;
    }
  }
  
  n_address_bytes = lenght / 256;
  for (i = address + 1, a = 0; i < address + lenght + 1 * n_address_bytes; i++, a++){
    if (a % 256 == 0){
      i++;
    }
    else{
      memory_map[i] = data[a];
    }
  }
}

String readString (int address){
  String data;
  for(i = 0; memory_map[address + 256 * i] >= 255; i++){
    lenght += memory_map[address + 256 * i];
  }
  n_address_bytes = lenght / 256;
  for (i = address + 1, a = 0; i < address + lenght + 1 * n_address_bytes; i++, a++){
    if (a % 256 == 0){
      i++;
    }
    else{
      data[a] = memory_map[i];
    }
  }
  return data;
}

int exponential(int base, int exponent){
  for(i = 1; exponent > 0; i *= base){}
  return i;
}
 void eepromClear(){
  for (i = 0; i < EEPROM_lenght; i++){
    EEPROM.write(i, 0);
  }
  EEPROM.commit();
 }
