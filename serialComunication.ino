void executeSerialCommands() {
  Serial.println(inputString);
  if (serialString[0].equalsIgnoreCase("info")) {
    reportStarting("Printing info");

    Serial.println("\n\nCheckpoints:");
    for (i = 0; i < 10; i++) {
      Serial.print(checkpoint_memory[i]);
      Serial.print(", ");
    }

    Serial.println("\n\nSettings:");
    for (i = 0; i < 10; i++) {
      Serial.print(settings[i]);
      Serial.print(", ");
    }
    Serial.println("\n\nSector loaded:");
    Serial.print(sector_loaded);

    Serial.println("\n\nCommands:");
    for (i = 0; i < 10; i++) {
      Serial.print(command_length[i]);
      Serial.print(", ");
    }
    reportEnding();
  }
  else if (serialString[0].equalsIgnoreCase("load")) {
    reportStarting("Loading sector");
    loadSector(serialString[1].toInt());
    Serial.println(serialString[1].toInt());
    for (i = 0; memory_map[i] != ""; i++) {
      Serial.println(memory_map[i]);
    }
    reportEnding();
  }
  else if (serialString[0].equalsIgnoreCase("update")) {
    Serial.print("Command updated: ");
    Serial.print(memory_map[serialString[1].toInt()]);
    Serial.print(" --> ");
    updateString(serialString[1].toInt(), serialString[2]);
    Serial.println(memory_map[serialString[1].toInt()]);
  }
  else if (serialString[0].equalsIgnoreCase("print")) {
    reportStarting("Printing sector");
    for (i = 0; memory_map[i] != ""; i++) {
      Serial.println(memory_map[i]);
    }
    reportEnding();
  }
  else if (serialString[0].equalsIgnoreCase("EEPROM")) {
    if (serialString[1].equalsIgnoreCase("update")) {
      updateEEPROM();
      Serial.println("EEPROM updated");
    }
    else if (serialString[1].equalsIgnoreCase("print")) {
      c = serialString[2].toInt();
      reportStarting("Start reading EEPROM");
      for (i = 0; i < c; i++) {
        Serial.print((int)EEPROM.read(i));
        Serial.print((char)9);
        Serial.println((char)EEPROM.read(i));
      }
      reportEnding();
    }
    else if (serialString[1].equalsIgnoreCase("clear")) {
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
  for (a = 0, i = 0; data[i] != '\0'; a++, i++) {
    subString = "";
    for (; (data[i] != ' ' || a == 2) && data[i] != '\0'; i++) {
      subString += data[i];
    }
    serialString[a] = subString;
    Serial.println(subString);
  }
  reportEnding();
}



void reportStarting(String comment) {
  Serial.println("--------------------------------");
  Serial.print(comment + "...\n");
  execution_time = micros();
  serial_reporter = 0;
}
void reportEnding() {
  Serial.print("\ndone ");
  Serial.print(micros() - execution_time);
  Serial.println("us\n");
}
void reportStep() {
  Serial.print(serial_reporter);
  Serial.print(", ");
  serial_reporter++;
}
