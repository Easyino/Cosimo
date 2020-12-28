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
  int address = calculateCheckpointAddress(sector);
  Serial.println("Command lengths:");
  for (r = address + address_bytes, c = 0; c < checkpoint_memory[sector] + address_bytes; r += command_length[c] + addressBytes(command_length[c]), c++) {
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
  for (i = 0; data[i] != '\0'; i++) {
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
