void eepromClear() {
  for (i = 0; i < EEPROM_length; i++) {
    EEPROM.write(i, 0);
  }
  EEPROM.commit();
}

void loadCheckpoints() {
  reportStarting("Loading checkpoints");
  for (q = 0, a = 1, r = EEPROM_offset; a != 0; q++) {
    for (; EEPROM.read(r) != 0 || EEPROM.read(r + 1) != 0; r++) {}
    if (EEPROM.read(r + 2) == 0) {
      a = 0;
    }
    r++;
    checkpoint_memory[q] = r;
    Serial.println(r);
  }
  
  sector_max = q - 1;
  reportEnding();
}

void loadSector(int sector) {
  reportStarting("Loading sector");
  if(sector_loaded != sector){
    sector_loaded = sector;
    checkpoint_jump = 0;
    for (i = 0; memory_map[i] != ""; i++) {
      memory_map[i] = "";
      memory_type[i] = -1;
    }
    for (i = checkpoint_memory[sector], q = 0; i < checkpoint_memory[sector + 1] - 2; i += a, q++) {
      memory_type[q] = EEPROM.read(i + 1);
      for (a = 2; EEPROM.read(a + i) != 0; a++) {
        memory_map[q] += (char)EEPROM.read(a + i);
      }
      if (memory_type[q] == password) {
        memory_map[q] = decryptString(memory_map[q]);
      }
      Serial.println(memory_map[q]);
    }
  }
  else {
    Serial.println("Already loaded");
  }
  
  reportEnding();
}

void updateCommand(int com, String data, int type) {
  if (type == password) {
    data = encryptString(data);
  }
  else if (type == command) {
    data = stringToCommand(data);
  }
  checkpoint_jump += data.length() - memory_map[com].length();
  memory_map[com] = data;
  memory_type[com] = type;
}

void updateEEPROM() {
  reportStarting("Updating eeprom");
  if (checkpoint_jump != 0) {
    Serial.print("Jump = ");
    Serial.println(checkpoint_jump);
    //shiftEEPROM (checkpoint_memory[sector_loaded + 1] - 1, checkpoint_jump);
    if (checkpoint_memory[sector_loaded + 1] == 0){
      Serial.println("New sector");
      checkpoint_memory[sector_loaded + 1] = checkpoint_memory[sector_max] + sectorLength() + 1;
      sector_max++;
    }
    else {
      Serial.println("Shifting checkpoints");
      for (i = sector_loaded + 1; checkpoint_memory[i] != 0; i++) {
        checkpoint_memory[i] += checkpoint_jump;
      }
    }
  }

  EEPROM.write(checkpoint_memory[sector_loaded] - 1, 0);
  for (i = checkpoint_memory[sector_loaded] + 2, r = 0; i < checkpoint_memory[sector_max]; i += a + 2, r++){
    EEPROM.write(i - 2, 0);
    EEPROM.write(i - 1, memory_type[r]);
    Serial.println(i - 2);
    for (a = 0; memory_map[r][a] != 0; a++){
      EEPROM.write(i + a, memory_map[r][a]);
    }
  }
  EEPROM.commit();
  reportEnding();
}

int sectorLength(){
  length = 0;
  for (c = 0; memory_map[c] != ""; c++, length += 2){
    length += memory_map[c].length();
  }
  return length;
}

void shiftEEPROM (int address, int jump) {
  if (jump > 0){
    for (i = 0; i < jump; i++) {
      c = (char)EEPROM.read(address + i);
      for (a = address + jump + i; a < checkpoint_memory[sector_max]; a += jump) {
        r = (char)EEPROM.read(a);
        EEPROM.write(a, c);
        c = r;
      }
    }
  }
  else {
    for (i = address; i < checkpoint_memory[sector_max]; i++){
      c = EEPROM.read(i - jump);
      EEPROM.write(i, c); 
    }
  }

  EEPROM.commit();
}

void loadTitles() {
  reportStarting("Loading titles");
  for (c = 2; checkpoint_memory[c] != 0; c++) {
    for (d = checkpoint_memory[c] + 2; EEPROM.read(d) != 0; d++) {
      elements_list[c - 1] += (char)EEPROM.read(d);
    }
    Serial.println(elements_list[c - 1]);
  }
  reportEnding();
}

void loadNetData() {
  loadSector(1);
  ext_ssid = memory_map[addrExtSSID];
  Serial.println("prendo SSID");
  ext_password = memory_map[addrExtPassword];
  Serial.println("prendo password");
}

void demoSectors() {
  String txt;
  for (e = 0; e < 10; e++) {
    txt = "Titolo settore ";
    txt += e;
    loadSector(e + 2);
    updateCommand(0, txt, text); //14 + 1
    updateCommand(1, "Resto dei comandi", text); //17 + 1   ---> 33 + 1
    updateEEPROM();
    ESP.wdtFeed();
  }
}