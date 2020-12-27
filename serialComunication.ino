void executeSerialCommands() {
  Serial.println(inputString);
  if (serialString[0].equalsIgnoreCase("load")) {
    loadSector(serialString[1].toInt());
    Serial.print("Sector loaded: ");
    Serial.println(serialString[1].toInt());
  }
  else if (serialString[0].equalsIgnoreCase("update")) {
    Serial.print("Command updated: ");
    Serial.print(memory_map[serialString[1].toInt()]);
    Serial.print(" --> ");
    updateString(serialString[1].toInt(), serialString[2]);
    Serial.println(memory_map[serialString[1].toInt()]);
  }
  else if (serialString[0].equalsIgnoreCase("info")) {
    reportStarting("Printing sector");
    for(i = 0; memory_map[i] != ""; i++){
      Serial.println(memory_map[i]);
    }
    reportEnding();
  }
  else if (serialString[0].equalsIgnoreCase("EEPROM")){
    if (serialString[1].equalsIgnoreCase("update")){
      updateEEPROM();
      Serial.println("EEPROM updated");
    }
    else if(serialString[1].equalsIgnoreCase("read")){
      c = serialString[2].toInt();
      reportStarting("Start reading EEPROM");
      for(i = 0; i < c; i++){
        Serial.print((int)EEPROM.read(i));
        Serial.print((char)9);
        Serial.println((char)EEPROM.read(i));
      }
      reportEnding();
    }
    else if(serialString[1].equalsIgnoreCase("clear")){
      eepromClear();
      Serial.println("EEPROM cleaned");
    }
  }
  else {
    Serial.println("Command not recongnised");
  }
  inputString = "";
  stringComplete = false;
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '\n') {
      stringComplete = true;
    }
    else {
      inputString += inChar;
    }
    Serial.print(inChar);
  }
}


void loadSerialCommands(String data) {
  reportStarting("Loading serial command");
  String subString;
  for (c = 0; serialString[c][0] != '\0'; c++) {
    serialString[c] = "";
  }
  reportStep();
  for (a = 0, i = 0; data[i] != '\0'; a++, i++) {
    subString = "";
    for (; data[i] != ' ' && data[i] != '\0'; i++) {
      subString += data[i];
    }
    serialString[a] = subString;
    Serial.println(subString);
  }
  reportEnding();
}



void reportStarting(String comment){
  Serial.println("--------------------------------");
  Serial.print(comment + "...\n");
  execution_time = millis();
  serial_reporter = 0;
}
void reportEnding(){
  Serial.print("\ndone ");
  Serial.print(millis() - execution_time);
  Serial.println("ms\n");
}
void reportStep(){
  Serial.print(serial_reporter);
  Serial.print(", ");
  serial_reporter++;
}
