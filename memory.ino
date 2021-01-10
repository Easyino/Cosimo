void eepromClear() {
  for (i = 0; i < EEPROM_length; i++) {
    EEPROM.write(i, 0);
  }
  EEPROM.commit();
}

void loadCheckpoints() {
  reportStarting("Loading checkpoints");
  for (r = EEPROM_offset, c = 0; EEPROM.read(r) != 0; r += a + addressBytes(a), c++) {
    checkpoint_memory[c] = calculatelength(r);
  }
  reportEnding();
}

void loadCommandlengths(int sector) {
  int address = calculateCheckpointAddress(sector);
  int address_bytes = addressBytes(checkpoint_memory[sector]);
  Serial.println("Command lengths:");
  for (r = address + address_bytes, c = 0; EEPROM.read(r) != 0; r += a + addressBytes(a), c++) {
    command_length[c] = calculatelength(r);
    Serial.print("r = ");
    Serial.println(r);
  }
  Serial.println("");
}


int calculateCheckpointAddress(int sector) {
  for (i = 0, a = EEPROM_offset; i < sector; i++) {
    a += checkpoint_memory[i] + addressBytes(checkpoint_memory[i]);
  }
  return a;
}

int calculatelength(int address) {
  if ((char)EEPROM.read(address) >= max_value_address) {
    a = -1;
  }
  else {
    for (i = 0, a = 0; (char)EEPROM.read(address + i) == max_value_address - 1; i++) {
      a += max_value_address * i;
    }
    a += (char)EEPROM.read(address);
    Serial.print("a = ");
    Serial.println(a);
  }
  return a;
}


void loadSector(int sector) {
  reportStarting("Loading sector");
  loadCommandlengths(sector);
  sector_loaded = sector;
  String data;
  for (i = 0; memory_map[i] != ""; i++) {
    memory_map[i] = "";
    memory_type[i] = -1;
  }
  int address = calculateCheckpointAddress(sector);
  int address_bytes = addressBytes(address);
  for (c = address + address_bytes + addressBytes(command_length[0]), i = 0, q = 0; c < checkpoint_memory[sector] + address + address_bytes; c += command_length[i] + addressBytes(command_length[i]), i++, q++) {
    if (command_length[i] == -1) {
      if (EEPROM.read(c) == 255) {
        memory_type[q] = password;
        q -= 2;
      }
      else {
        if (EEPROM.read(c) == 254 || EEPROM.read(c) == 253){
          q -= 2;
        }
        else{
          memory_type[q] = command;
        }
        memory_map[q] += (char)EEPROM.read(c);
      }
    }
    else {
      data = "";
      for (r = 0; r < command_length[i]; r++) {
        data += (char)EEPROM.read(r + c);
      }
      if (memory_type[q] == password){
        memory_map[q] = decryptString(data);
      }
      else{
        memory_map[q] = data;
        memory_type[q] = text;
      }
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
  int sign_indicator = jump / abs(jump);
  for (i = 0; i != jump; i += sign_indicator) {
    c = (char)EEPROM.read(address + jump + i);
    for (a = address + jump + i; EEPROM.read(a) != 0; a += abs(jump)) {
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
    if (memory_type[i] == password) {
      data += (char)255;
    }
    if (memory_type[i] != command) {
      if (memory_type[i] == password){
        data += writelength(memory_map[i].length() + 28);
      }
      else{
        data += writelength(memory_map[i].length());
      }
    }
    if (memory_type[i] != password){
      data += memory_map[i];
    }
    else{
      data += encryptString(memory_map[i]);
    }
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

void updateCommand(int command, String data, int type) {
  if (type == command){
    data = stringToCommand(data);
  }
  checkpoint_memory[sector_loaded] += rawLength(data, type) - rawLength(memory_map[command], memory_type[command]);
  memory_map[command] = data;
  memory_type[command] = type;
}

int rawLength(String data, int type) {
  int length;
  if (data.length() != 0) {
    length = data.length();
    if (type == password){
      length += 29;
    }
    if (type != command){
      length += length / max_value_address + 1;
    }
    return length;
  }
  return 0;
}

int addressBytes(int length) {
  int n;
  if (length != -1) {
    for (n = 0; length >= max_value_address; length -= max_value_address, n++) {}
    if (length != 0) {
      n++;
    }
  }
  else {
    n = 2;
  }
  return n;
}
