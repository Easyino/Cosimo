void executeSerialCommands() {
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

    Serial.println("\n\nCommand lengths:");
    for (i = 0; i < 10; i++) {
      Serial.print(command_length[i]);
      Serial.print(", ");
    }

    Serial.println("\n\nTypes:");
    for (i = 0; i < 10; i++) {
      Serial.print(memory_type[i]);
      Serial.print(", ");
    }
    reportEnding();
  }


  else if (serialString[0].equalsIgnoreCase("load")) {
    if (serialString[1].equalsIgnoreCase("checkpoints")) {
      loadCheckpoints();
    }
    else {
      loadSector(serialString[1].toInt());
      for (i = 0; memory_map[i] != ""; i++) {
        Serial.print((int)i);
        Serial.print(" ");
        Serial.print(data_types[memory_type[i]]);
        Serial.print((char)9);
        if (memory_type[i] == command) {
          Serial.println(commandToString(memory_map[i]));
        }
        else {
          Serial.println(memory_map[i]);
        }
      }
    }
  }
  else if (serialString[0].equalsIgnoreCase("update")) {
    Serial.print("Command updated: ");
    Serial.print(memory_map[serialString[1].toInt()]);
    Serial.print(" --> ");
    updateCommand(serialString[1].toInt(), serialString[3], dataTypes(serialString[2]));
    Serial.println(memory_map[serialString[1].toInt()]);
  }

  else if (serialString[0].equalsIgnoreCase("print")) {
    reportStarting("Printing sector");
    for (i = 0; memory_map[i] != ""; i++) {
      Serial.print((int)i);
      Serial.print(" ");
      Serial.print(data_types[memory_type[i]]);
      Serial.print((char)9);
      if (memory_type[i] == command) {
        Serial.println(commandToString(memory_map[i]));
      }
      else {
        Serial.println(memory_map[i]);
      }
    }
    reportEnding();
  }
  else if (serialString[0].equalsIgnoreCase("keyboard")) {
    if (serialString[1].equalsIgnoreCase("send")) {
      reportStarting("Sending on the keyboard");
      Serial.print(data_types[memory_type[serialString[2].toInt()]]);
      Serial.print((char)9);
      Serial.println(memory_map[serialString[2].toInt()]);
      sendSlave(memory_map[serialString[2].toInt()], memory_type[serialString[2].toInt()]);
      reportEnding();
    }
    else if (serialString[1].equalsIgnoreCase("request")){
      reportStarting("Requesting to slave");
      Serial.println(checkReady());
      reportEnding();
    }
  }


  else if (serialString[0].equalsIgnoreCase("encrypt")) {
    memory_map[serialString[1].toInt()] = encryptString(serialString[2]);
    Serial.println(memory_map[serialString[1].toInt()]);
  }
  else if (serialString[0].equalsIgnoreCase("decrypt")) {
    memory_map[serialString[1].toInt()] = decryptString(memory_map[serialString[1].toInt()]);
    Serial.println(memory_map[serialString[1].toInt()]);
  }
  else if (serialString[0].equalsIgnoreCase("key")) {
    setMasterKey(serialString[1]);
  }


  else if (serialString[0].equalsIgnoreCase("EEPROM")) {
    if (serialString[1].equalsIgnoreCase("update")) {
      updateEEPROM();
      Serial.println("EEPROM updated");
    }
    else if (serialString[1].equalsIgnoreCase("print")) {
      c = serialString[2].toInt();
      reportStarting("Start reading EEPROM");
      for (i = EEPROM_offset; i < c + EEPROM_offset; i++) {
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
  

  else if (serialString[0].equalsIgnoreCase("wifi")) {
    if (serialString[1].equalsIgnoreCase("connect")) {
      tryConnect();

    }
    else if (serialString[1].equalsIgnoreCase("create")) {
      createNetwork();

    }
    else if (serialString[1].equalsIgnoreCase("stop")) {
      WiFi.softAPdisconnect (true);
      Serial.println("WIFI access point has been stopped");
    }

    if (serialString[1].equalsIgnoreCase("mode")) {
      EEPROM.write(0, serialString[2].toInt());
      EEPROM.commit();
      Serial.print("setting wifi mode to ");
      Serial.println(serialString[2].toInt());
    }
  }

  else if (serialString[0].equalsIgnoreCase("reboot")) {
    ESP.restart();
  }

  else if (serialString[0].equalsIgnoreCase("OTA")) {
    if (OTAupdate()) {
      Serial.println("Ready to be updated through the net");
    }
    else {
      Serial.println("Failed");
    }
  }
  else if (serialString[0].equalsIgnoreCase("interface")){
    interface = serialString[1].toInt();
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
    for (; (data[i] != ' ' || a == 3) && data[i] != '\0'; i++) {
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
  Serial.println("\n--------------------------------\n");
}
void reportStep() {
  Serial.print(serial_reporter);
  Serial.print(", ");
  serial_reporter++;
}
