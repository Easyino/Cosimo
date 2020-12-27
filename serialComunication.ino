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
  else {
    Serial.println("Comando non riconosciuto");
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
  }
}

void loadSerialCommands(String data) {
  String subString;
  for (c = 0; serialString[c] != '\0'; c++) {
    serialString[c] = "\0";
  }
  for (a = 0, i = 0; data[i] != '\0'; a++) {
    for (i = 0; data[i] != ' ' || data[i] != '\0'; i++) {
      subString += data[i];
    }
    serialString[a] = subString;
  }
}



void reportStarting(String comment){
  Serial.print(comment + "... ");
  execution_time = millis();
  serial_reporter = 0;
}
void reportEnding(){
  Serial.print("done ");
  Serial.print(millis() - execution_time);
  Serial.println("ms");
}
void reportStep(){
  Serial.print(serial_reporter);
  Serial.print(", ");
  serial_reporter++;
}
