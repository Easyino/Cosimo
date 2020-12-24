void eepromClear() {
  for (i = 0; i < EEPROM_lenght; i++) {
    EEPROM.write(i, 0);
  }
  EEPROM.commit();
}

int calculateCheckpointAddress(int sector) {
  for (i = 0, a = 0; i < sector; i++) {
    a += checkpoint_memory[i]
  }
  return a;
}
int calculateCommandAddress(int command){
  for (i = 0, a = 0; i < command; i++) {
    a += command_lenght[i]
  }
  return a;
}

int calculateLenght(int address) {
  if (EEPROM.read(address + i) >= max_value_address) {
    a = 1;
  }
  else {
    for (i = 0, a = 0; EEPROM.read(address + i) == max_value_address; i++) {
      a += address + max_value_address * i;
    }
    a += EEPROM.read(address);
  }
  return a;
}

void loadCheckpoints() {
  for (r = 0, c = 0; EEPROM.read(r) != 0; r += a, c++) {
    checkpoint_memory[c] = calculateLenght(r);
  }
}

byte loadCommandLenghts(int sector) {
  byte arr[] = 0;
  for (r = checkpoint_memory[sector] / max_value_address + 1, c = 0; r < checkpoint_memory[sector]; r += a, c++) {
    arr[c] = calculateLenght(r);
  }
  return arr;
}

byte loadSector(int sector) {
  byte arr[];
  loadCommandLenght(sector);
  int address = calculateCheckpointAddress(sector) + checkpoint_memory[sector] / max_value_address + 1;
  for (a = 0; command_lenght[a] != 0; a++) {
    for (i = address + command_lenght[a] / max_value_address + 1; i < command_lenght[a]; i++) {
      arr[i] = EEPROM.read(i);
    }
  }
  return arr;
}

String titleSector(int sector) {
  String data;
  int address = calculateCheckpointAddress(sector);
  a = EEPROM.read(address + 1);
  for (i = 0; i < a; i++) {
    data += EEPROM.read(address + 1 + i);
  }
  return data;
}

void loadNetData() {
  ext_ssid = readString(settings, addrExtSSID);
  Serial.println("prendo SSID");
  ext_password = readString(settings, addrExtPassword);
  Serial.println("prendo password");
}

void addString (byte arr[], String data) {
  //  address = calculateLenght(arr[], 0) + 1;
  //  for (command_lenght = 0; data[command_lenght] > 0; command_lenght++) {}
  //  for (i = command_lenght, a = 0; i > 0; i -= max_value_address, a++) {
  //    if (i > max_value_address - 1) {
  //      arr[address + max_value_address * a] = max_value_address - 1;
  //    }
  //    else {
  //      arr[address + max_value_address * a] = i;
  //    }
  //  }
  //  if (sector_lenght / max_value_address != sector_lenght + command_lenght / max_value_address) {
  //    /// Update sector
  //  }
  //  else {
  //    for (i = 0; arr[i] == max_value_address - 1; i++) {
  //    }
  //    i--;
  //    arr[i] = sector_lenght + command_lenght % max_value_address;
  //  }
  //
  //  n_address_bytes = command_lenght / max_value_address;
  //  for (i = address + 1, a = 0; i < address + command_lenght + 1 * n_address_bytes; i++, a++) {
  //    if (a % max_value_address == 0) {
  //      i++;
  //    }
  //    else {
  //      arr[i] = data[a];
  //    }
  //  }
}

String readString (byte arr[], int command) {
  String data;
  //  address = calculateCommandAddress(arr, command);
  //  command_lenght = calculateLenght(arr, address);
  //  n_address_bytes = command_lenght / max_value_address;
  //  for (i = address + 1; i < address + command_lenght + 1 * n_address_bytes; i++) {
  //    if (a % max_value_address == 0) {
  //    }
  //    else {
  //      data += arr[i];
  //    }
  //  }
  //  return data;
}
