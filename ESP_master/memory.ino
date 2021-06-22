void eepromClear() {
  int i;
  for (i = EEPROM_offset; i < EEPROM_length; i++) {
    EEPROM.write(i, 0);
  }
  setDefault();
}


void loadCheckpoints() {
  int a, e, r;
  reportStarting("Loading checkpoints");
  for (e = 0, a = 1, r = EEPROM_offset; a != 0; e++) {
    for (; EEPROM.read(r) != 0 || EEPROM.read(r + 1) != 0; r++) {}
    if (EEPROM.read(r + 2) == 0) {
      a = 0;
    }
    r++;
    checkpoint_memory[e] = r;
    Serial.println(r);
  }
  
  sector_max = e - 1;
  reportEnding();
}

void loadSector(int sector) {
  int i, a, e;
  reportStarting("Loading sector");
  checkpoint_jump = 0;
  if (sector > sector_max){
    sector = sector_max;
    Serial.println("Not enough sectors saved");
  }
  sector_loaded = sector;
  for (i = 0; memory_map[i] != ""; i++) {
    memory_map[i] = "";
    memory_type[i] = -1;
  }
  for (i = checkpoint_memory[sector], e = 0; i < checkpoint_memory[sector + 1] - 2; i += a, e++) {
    memory_type[e] = EEPROM.read(i + 1);
    for (a = 2; EEPROM.read(a + i) != 0; a++) {
      memory_map[e] += (char)EEPROM.read(a + i);
    }
    if (memory_type[e] == password) {
      memory_map[e] = decryptString(memory_map[e]);
    }
    Serial.println(memory_map[e]);
  }
  reportEnding();
}

void updateCommand(int com, String data, int type) {
  if (type == command) {
    data = stringToCommand(data);
  }
  checkpoint_jump += rowLength(data, type) - rowLength(memory_map[com], memory_type[com]);
  
  memory_map[com] = data;
  memory_type[com] = type;
}

int rowLength(String data, int type){
  int length;
  if(data != ""){
    length = 2;
    if (type == password){
      length += 28;
    }
    length += data.length();
    return length;
  }
  return 0;
}

int sectorLength(){
  int c;
  int length = 0;
  for (c = 0; memory_map[c] != ""; c++){
    length += rowLength(memory_map[c], memory_type[c]);
  }
  Serial.print("Sector length = ");
  Serial.println(length);
  return length;
}

void updateEEPROM() {
  int a, i, r;
  reportStarting("Updating eeprom");
  if (checkpoint_jump != 0) {
    Serial.print("Jump = ");
    Serial.println(checkpoint_jump);
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
    shiftEEPROM (checkpoint_memory[sector_loaded + 1] - (checkpoint_jump + 1), checkpoint_jump);
  }

  EEPROM.write(checkpoint_memory[sector_loaded] - 1, 0);
  for (i = checkpoint_memory[sector_loaded] + 2, r = 0; i < checkpoint_memory[sector_loaded + 1]; i += a + 2, r++){
    String data = memory_map[r];
    if (data != ""){
      EEPROM.write(i - 2, 0);
      EEPROM.write(i - 1, memory_type[r]);
      Serial.println(i - 2);
      if (memory_type[r] == password){
        data = encryptString(data);
      }
      for (a = 0; data[a] != 0; a++){
        EEPROM.write(i + a, data[a]);
      }
    }
    else {
      a = -2;
    }
  }
  EEPROM.commit();
  reportEnding();
}

void shiftEEPROM (int address, int jump) {
  int i, c, a, r;
  Serial.print("from ");
  Serial.print(address);
  Serial.print(" to ");
  Serial.println(address + jump);
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
    for (i = address + jump; i < checkpoint_memory[sector_max]; i++){
      c = EEPROM.read(i - jump);
      EEPROM.write(i, c); 
    }
  }
  EEPROM.commit();
}

byte defaultPar[]{0, 0, 5, 1, 5};
void setDefault(){
  int i;
  for(i = 1; i < EEPROM_offset; i++){
    EEPROM.write(i, defaultPar[i]);
  }
  EEPROM.commit();
}
void eepromPar(int address){
  int par = EEPROM.read(address);
  switch(address){
    case 1:{
      if (par) {
        tryConnect();
      }
      else {
        createNetwork();
      }
      break;
    }
    case 2:{
      display.setContrast(par * 51);
      break;
    }
    case 3:{
      if(par){
        display.flipScreenVertically();
      }
      else {
        display.resetOrientation();
      }
      orientation = !par;
      break;
    }
    case 4:{
      n_rows = par;
      loaded_interface = 0;
      break;
    }
  }
}


void loadTitles() {
  int c, d;
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
  ext_ssid = memory_map[0];
  Serial.println("prendo SSID");
  ext_password = memory_map[1];
  Serial.println("prendo password");
}

void demoSectors() {
  int e;
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