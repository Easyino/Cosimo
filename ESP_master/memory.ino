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
  for (i = 0; command_length[i] != 0; i++) {
    command_length[i] = 0;
  }
  Serial.println("Command lengths:");
  for (r = address + address_bytes, c = 0; r < address + checkpoint_memory[sector]; r += a + addressBytes(a), c++) {
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
  if (sector < 0) { //injection code protection
    Serial.println("Protection triggered");
    eepromClear();
    ESP.restart();
  }
  reportStarting("Loading sector");
  loadCommandlengths(sector);
  sector_loaded = sector;
  checkpoint_jump = 0;
  String data;
  bool number = false;
  for (i = 0; memory_map[i] != ""; i++) {
    memory_map[i] = "";
    memory_type[i] = -1;
  }
  int address = calculateCheckpointAddress(sector);
  int address_bytes = addressBytes(address);
  for (c = address + address_bytes, i = 0, q = 0; command_length[i] != 0; i++, q++) {
    Serial.print("i = ");
    Serial.print(i);
    Serial.print("    q = ");
    Serial.print(q);
    Serial.print("    c = ");
    Serial.println(c);
    if (command_length[i] == -1) {
      Serial.print("EEPROM value = ");
      Serial.println(EEPROM.read(c));
      if (EEPROM.read(c) == 255 && number == false) {
        memory_type[q] = password;
        q--;
      }
      else {
        if (number == true) {
          number = false;
        }
        else {
          if (EEPROM.read(c) == 254) {
            number = true;
            q--;
          }
          else if (EEPROM.read(c) == 253) {
            q--;
          }
          else {
            memory_type[q] = command;
          }
        }
        memory_map[q] += (char)EEPROM.read(c);
      }
      c++;
    }
    else {
      c += addressBytes(command_length[i]);
      data = "";
      for (r = 0; r < command_length[i]; r++) {
        data += (char)EEPROM.read(r + c);
      }
      if (memory_type[q] == password) {
        memory_map[q] = decryptString(data);
      }
      else {
        memory_map[q] = data;
        memory_type[q] = text;
      }
      c += command_length[i];
    }
  }
  reportEnding();
}


void updateEEPROM() {
  String data = rawData();
  Serial.print("Raw data = ");
  Serial.println(data);
  int address = calculateCheckpointAddress(sector_loaded);

  Serial.print("Address = ");
  Serial.println(address);
  Serial.print("Jump = ");
  Serial.println(checkpoint_jump);

  shiftEEPROM(address + checkpoint_memory[sector_loaded] - checkpoint_jump, checkpoint_jump);
  address -= checkpoint_memory[sector_loaded] / max_value_address;
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
      if (memory_type[i] == password) {
        data += writelength(memory_map[i].length() + 28);
      }
      else {
        data += writelength(memory_map[i].length());
      }
    }
    if (memory_type[i] != password) {
      data += memory_map[i];
    }
    else {
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

void updateCommand(int com, String data, int type) {
  int jump;
  Serial.print(data_types[type] + "  ");
  if (type == command) {
    data = stringToCommand(data);
  }
  Serial.println(data);
  jump = rawLength(data, type) - rawLength(memory_map[com], memory_type[com]);
  checkpoint_jump += jump;
  checkpoint_memory[sector_loaded] += jump;
  memory_map[com] = data;
  memory_type[com] = type;
}

int rawLength(String data, int type) {
  int length;
  if (data.length() != 0) {
    length = data.length();
    if (type == password) {
      length += 29;
    }
    if (type != command) {
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
