void loadNetData() {
  ext_ssid = readString(settings, addrExtSSID);
  Serial.println("prendo SSID");
  ext_password = readString(settings, addrExtPassword);
  Serial.println("prendo password");
}

void writeString (byte arr[], int address, String data) {
  for (lenght = 0; data[lenght] > 0; lenght++) {}
  for (i = lenght, a = 0; i > 0; i -= max_value_address, a++) {
    if (i > max_value_address - 1) {
      arr[address + max_value_address * a] = max_value_address - 1;
    }
    else {
      arr[address + max_value_address * a] = i;
    }
  }

  n_address_bytes = lenght / max_value_address;
  for (i = address + 1, a = 0; i < address + lenght + 1 * n_address_bytes; i++, a++) {
    if (a % max_value_address == 0) {
      i++;
    }
    else {
      arr[i] = data[a];
    }
  }
}

String readString (byte arr[], int command) {
  String data;
  address = calculateCommandAddress(arr, command);
  lenght = calculateLenght(arr, address);
  n_address_bytes = lenght / max_value_address;
  for (i = address + 1; i < address + lenght + 1 * n_address_bytes; i++) {
    if (a % max_value_address == 0) {
    }
    else {
      data += arr[i];
    }
  }
  return data;
}

void eepromClear() {
  for (i = 0; i < EEPROM_lenght; i++) {
    EEPROM.write(i, 0);
  }
  EEPROM.commit();
}

void loadSector(byte arr[], int sector) {
  checkpoint_address = calculateCheckpointAddress(sector);
  for(i = 0, lenght = 1; EEPROM.read(checkpoint_address + i) >= max_value_address; i++, lenght += EEPROM.read(checkpoint_address + i)){
  }
  lenght = EEPROM.read(checkpoint_address) + 1;
  for (i = checkpoint_memory[sector], a = 0; a < lenght; i++, a++) {
    memory_map[a] = EEPROM.read(i);
  }
  sectorLenght = 
}

String titleSector(int sector) {
  String data;
  int address = calculateCheckpointAddress(sector);
  lenght = EEPROM.read(address + 1);
  for (i = 0; i < lenght; i++) {
    data += EEPROM.read(address + 1 + i);
  }
  return data;
}

int calculateCheckpointAddress(int sector) {
  for (i = 0, a = 0; i < sector; i++) {
    a += checkpoint_memory[i]
  }
  return a;
}

int calculateCommandAddress(byte arr[], int command) {
  for(i = 0, a = 1; i < command + 1; i++){
    if (arr[a] < 128){
      a += calculateLenght(arr, a);
    }
    else{
      a++;
    }
  }
  return a;
}

int calculateLenght(byte arr[], int address){
  for (i = 0, a = 0; arr[address + max_value_address * i] >= max_value_address - 1; i++) {
    a += arr[address + max_value_address * i];
  }
  return a;
}
