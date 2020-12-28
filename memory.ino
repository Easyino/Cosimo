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

void eepromClear() {
  for (i = 0; i < EEPROM_length; i++) {
    EEPROM.write(i, 0);
  }
  EEPROM.commit();
}

void loadCheckpoints() {
  reportStarting("Loading checkpoints");
  reportStep();
  for (r = 0, c = 0; (char)EEPROM.read(r) != 0; r += a, c++) {
    checkpoint_memory[c] = calculatelength(r);
  }
  reportEnding();
}

void loadCommandlengths(int sector) {
  int address_bytes = addressBytes(checkpoint_memory[sector]);
  Serial.println("Command lengths:");
  for (r = address_bytes + 1, c = 0; r < checkpoint_memory[sector] + address_bytes; r += a + addressBytes(a), c++) {
    command_length[c] = calculatelength(r);
    Serial.print(a);
    Serial.print(", ");
  }
  Serial.println("");
}


int calculateCheckpointAddress(int sector) {
  for (i = 0, a = 0; i < sector; i++) {
    a += checkpoint_memory[i] + addressBytes(checkpoint_memory[i]);
  }
  return a;
}

int calculatelength(int address) {
  if ((char)EEPROM.read(address + i) >= max_value_address) {
    a = 1;
  }
  else {
    for (i = 0, a = 0; (char)EEPROM.read(address + i) == max_value_address; i++) {
      a += address + max_value_address * i;
    }
    a += (char)EEPROM.read(address);
  }
  return a;
}


void loadSector(int sector) {
  reportStarting("Loading sector");
  loadCommandlengths(sector);
  reportStep();
  sector_loaded = sector;
  for (i = 0; memory_map[i] != ""; i++) {
    memory_map[i] = "";
  }
  reportStep();
  int address = calculateCheckpointAddress(sector) + 1;
  reportStep();
  for (c = 0, r = 0; c < checkpoint_memory[sector]; c += command_length[r] + addressBytes(command_length[r]), r++) {
    for (i = address + addressBytes(command_length[r]); i < command_length[r] + addressBytes(command_length[r]); i++) {
      memory_map[r] += (char)EEPROM.read(i);
    }
  }
  reportEnding();
}


void updateEEPROM() {
  String data = rawData();
  int length;
  Serial.print("Raw data = ");
  Serial.println(data);
  int address = calculateCheckpointAddress(sector_loaded);
  Serial.print("Address end = ");
  Serial.println(address);
  Serial.print("length = ");
  Serial.println(calculatelength(address));
  shiftEEPROM(address + checkpoint_memory[sector_loaded], checkpoint_memory[sector_loaded] - calculatelength(address));
  address -= checkpoint_memory[sector_loaded] / max_value_address;
  Serial.print("Address start = ");
  Serial.println(address);
  length = checkpoint_memory[sector_loaded];
  for (i = 0; i < length; i++) {
    EEPROM.write(address + i, data[i]);
  }
  EEPROM.commit();
}

void shiftEEPROM(int address, int jump) {
  for (i = 0; i != jump; i += jump / abs(jump)) {
    c = (char)EEPROM.read(address + jump + i);
    for (a = address + jump + i; r != 0; a += abs(jump)) {
      r = (char)EEPROM.read(a);
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
  data += (char)(length % max_value_address);
  return data;
}


void loadNetData() {
  loadSector(1);
  ext_ssid = memory_map[addrExtSSID];
  Serial.println("prendo SSID");
  ext_password = memory_map[addrExtPassword];
  Serial.println("prendo password");
}

void updateString(int command, String data) {
  checkpoint_memory[sector_loaded] += rawLength(data) - rawLength(memory_map[command]);
  memory_map[command] = data;
}

int rawLength(String data) {
  int length;
  if (data.length() != 0) {
    length = data.length();
    length += length / max_value_address + 1;
    return length;
  }
  return 0;
}

int addressBytes(int length) {
  int n;
  for (n = 0; length >= max_value_address; length -= max_value_address, n++) {}
  if (length != 0){
    a++;
  }
  return n;
}
