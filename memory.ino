void eepromClear() {
  for (i = 0; i < EEPROM_lenght; i++) {
    EEPROM.write(i, 0);
  }
  EEPROM.commit();
}

int calculateCheckpointAddress(int sector) {
  for (i = 0, a = 0; i < sector; i++) {
    a += checkpoint_memory[i];
  }
  return a;
}
int calculateCommandAddress(int command) {
  for (i = 0, a = 0; i < command; i++) {
    a += command_lenght[i];
  }
  return a;
}

void loadSector(int sector) {
  sector_loaded = sector;
  loadCommandLenghts(sector);
  int address = calculateCheckpointAddress(sector) + checkpoint_memory[sector] / max_value_address + 1;
  for (a = 0; command_lenght[a] != 0; a++) {
    for (i = address + command_lenght[a] / max_value_address + 1; i < command_lenght[a]; i++) {
      memory_map[i] = EEPROM.read(i);
    }
  }
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
  loadSector(1);
  ext_ssid = readString(addrExtSSID);
  Serial.println("prendo SSID");
  ext_password = readString(addrExtPassword);
  Serial.println("prendo password");
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

void loadCommandLenghts(int sector) {
  for (r = checkpoint_memory[sector] / max_value_address + 1, c = 0; r < checkpoint_memory[sector]; r += a, c++) {
    command_lenght[c] = calculateLenght(r);
  }
}

String readString (int command) {
  String data;
  int address = calculateCommandAddress(command);
  for (i = address, a = 0; i < command_lenght[command]; i++, a++) {
    data[a] = memory_map[i];
  }
  return data;
}

void updateString(int command, String data) {
  int address = calculateCommandAddress(command);
  for (i = 0; data[i] > 0; i++) {}
  int lenght = i;
  shiftRAM(address + command_lenght[command], lenght - command_lenght[command]);
  checkpoint_memory[sector_loaded] += lenght - checkpoint_memory[sector_loaded];
  command_lenght[command] = i;
  for (c = 0, r = address; c < lenght; c++, r++) {
    memory_map[r] = data[c];
  }
}

void addString(int command, String data) {
  int address = calculateCommandAddress(command);
  for (i = 0; data[i] > 0; i++) {}
  int lenght = i;
  shiftRAM(address, lenght);
  checkpoint_memory[sector_loaded] += lenght;
  c = command_lenght[address + 1];
  for (a = address + 1; r != 0; a++) {
    r = command_lenght[a];
    command_lenght[a] = c;
    c = r;
  }
  command_lenght[address] = lenght;
  for (c = 0, r = address; c < lenght; c++, r++) {
    memory_map[r] = data[c];
  }
}

void shiftRAM(int address, int jump) {
  for (i = 0; i < jump; i++) {
    c = memory_map[address + jump + i];
    for (a = address + jump + i; r != 0; a += jump) {
      r = memory_map[a];
      memory_map[a] = c;
      c = r;
    }
  }
}

void updateEEPROM() {
  
}
