/*void eepromClear() {
  for (i = 0; i < EEPROM_length; i++) {
    EEPROM.write(i, 0);
  }
  EEPROM.commit();
  }

  void loadCheckpoints() {
  for (r = 0, c = 0; EEPROM.read(r) != 0; r += a, c++) {
    checkpoint_memory[c] = calculatelength(r);
  }
  }

  void loadCommandlengths(int sector) {
  for (r = checkpoint_memory[sector] / max_value_address + 1, c = 0; r < checkpoint_memory[sector]; r += a, c++) {
    command_length[c] = calculatelength(r);
  }
  }

  void loadSector(int sector) {
  sector_loaded = sector;
  loadCommandlengths(sector);
  int address = calculateCheckpointAddress(sector) + checkpoint_memory[sector] / max_value_address + 1;
  for (a = 0; command_length[a] != 0; a++) {
    for (i = address + command_length[a] / max_value_address + 1; i < command_length[a]; i++) {
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

  int calculateCheckpointAddress(int sector) {
  for (i = 0, a = 0; i < sector; i++) {
    a += checkpoint_memory[i];
  }
  return a;
  }
  int calculateCommandAddress(int command) {
  for (i = 0, a = 0; i < command; i++) {
    a += command_length[i];
  }
  return a;
  }

  int calculatelength(int address) {
  if (EEPROM.read(address + i) >= max_value_address) {
    a = 1;
  }
  else {
    for (i = 0, a = 0; EEPROM.read(address + i) == max_value_address; i++) {
      a += address + max_value_address * i;
    }
    a += EEPROM.read(address) + i;
  }
  return a;
  }

  String readString (int command) {
  String data;
  int address = calculateCommandAddress(command);
  for (i = address, a = 0; i < command_length[command]; i++, a++) {
    data[a] = memory_map[i];
  }
  return data;
  }

  void updateString(int command, String data) {
  int address = calculateCommandAddress(command);
  for (i = 0; data[i] > 0; i++) {}
  int length = i;
  shiftRAM(address + command_length[command], length - command_length[command]);
  checkpoint_memory[sector_loaded] += length - checkpoint_memory[sector_loaded];
  command_length[command] = i;
  for (c = 0, r = address; c < length; c++, r++) {
    memory_map[r] = data[c];
  }
  }

  void addString(int command, String data) {
  int address = calculateCommandAddress(command);
  for (i = 0; data[i] > 0; i++) {}
  int length = i;
  shiftRAM(address, length);
  checkpoint_memory[sector_loaded] += length;
  c = command_length[address + 1];
  for (a = address + 1; r != 0; a++) {
    r = command_length[a];
    command_length[a] = c;
    c = r;
  }
  command_length[address] = length;
  for (c = 0, r = address; c < length; c++, r++) {
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
*////////////////////////// Nuovo metodo di memorizzazione..

void loadCheckpoints() {
  for (r = 0, c = 0; EEPROM.read(r) != 0; r += a, c++) {
    checkpoint_memory[c] = calculatelength(r);
  }
}

void loadCommandlengths(int sector) {
  for (r = checkpoint_memory[sector] / max_value_address + 1, c = 0; r < checkpoint_memory[sector]; r += a, c++) {
    command_length[c] = calculatelength(r);
  }
}



int calculateCheckpointAddress(int sector) {
  for (i = 0, a = 0; i < sector; i++) {
    a += checkpoint_memory[i] + checkpoint_memory[i] / max_value_address + 1;
  }
  return a;
}

int calculatelength(int address) {
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



void loadSector(int sector) {
  loadCommandlengths(sector);
  sector_loaded = sector;
  for (i = 0; command_length[i] != 0; i++) {
    memory_map[i] = "";
  }
  int address = calculateCheckpointAddress(sector) + 1;
  for (c = 0, r = 0; c < checkpoint_memory[sector]; c += command_length[r] + command_length[r] / max_value_address + 1, r++) {
    for (i = address + command_length[r] / max_value_address + 1, a = 0; a < command_length[r]; i++, a++) {
      memory_map[r] += EEPROM.read(i);
    }
  }
}



void updateEEPROM() {
  String data = rawData();
  int address = calculateCheckpointAddress(sector_loaded);
  shiftEEPROM(address + checkpoint_memory[sector_loaded], checkpoint_memory[sector_loaded] - calculatelength(address));
  address -= checkpoint_memory[sector_loaded] / max_value_address;
  for (i = 0; i < checkpoint_memory[sector_loaded]; i++) {
    EEPROM.write(address + i, data[i]);
  }
  EEPROM.commit();
}

void shiftEEPROM(int address, int jump) {
  for (i = 0; i < jump; i++) {
    c = EEPROM.read(address + jump + i);
    for (a = address + jump + i; r != 0; a += jump) {
      r = EEPROM.read(a);
      EEPROM.write(a, c);
      c = r;
    }
  }
  EEPROM.commit();
}

String rawData() {
  String data;
  data += writelength(checkpoint_memory[sector_loaded]);
  for (i = 0; memory_map[i] != ""; i++) {
    data += writelength(memory_map[i].length());
    data += memory_map[i];
  }
  return data;
}

String writelength(int length) {
  String data;
  for (r = 0; r < length / max_value_address; r++) {
    data += r * max_value_address;
  }
  data += length % max_value_address;
  return data;
}

void loadNetData() {
  loadSector(1);
  ext_ssid = memory_map[addrExtSSID];
  Serial.println("prendo SSID");
  ext_password = memory_map[addrExtPassword];
  Serial.println("prendo password");
  }
