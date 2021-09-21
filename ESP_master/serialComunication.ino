void executeSerialCommands() {
  int i, c;
  if (serialString[0].equalsIgnoreCase("info")) {
    reportStarting("Printing info");

    Serial.println("\n\nCheckpoints:");
    for (i = 0; checkpoint_memory[i] != 0; i++) {
      Serial.print(checkpoint_memory[i]);
      Serial.print(", ");
    }

    Serial.println("\n\nCheckpoint jump:");
    Serial.print(checkpoint_jump);

    Serial.println("\n\nSector loaded:");
    Serial.print(sector_loaded);

    Serial.println("\n\nSector max:");
    Serial.print(sector_max);

    Serial.println("\n\nTypes:");
    for (i = 0; memory_type[i] != 0; i++) {
      Serial.print(memory_type[i]);
      Serial.print(", ");
    }

    Serial.println("\n\nInterface:");
    Serial.print(interface);

    Serial.println("\n\nElement counter");
    Serial.print(element_counter);

    Serial.println("\n\nInterface elements:");
    Serial.println("Limit | Aligned | Title | x | y | Content");
    for (i = 0; i < element_counter; i++) {
      Serial.print(element[i].limit);
      Serial.print(" | ");
      Serial.print(element[i].aligned);
      Serial.print(" | ");
      Serial.print(element[i].font);
      Serial.print(" | ");
      Serial.print(element[i].x);
      Serial.print(" | ");
      Serial.print(element[i].y);
      Serial.print(" | ");
      Serial.println(element[i].data);
    }

    Serial.println("\n\nInterface history:");
    for (i = 0; i < 5; i++) {
      Serial.print(history_interface[i]);
      Serial.print(", ");
    }

    Serial.println("\n\nPrevious interface:");
    Serial.print(previous_interface);

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
    updateCommand(serialString[1].toInt(), serialString[3], dataTypes(serialString[2]));// command number, data type, data
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
      sendSlave(memory_type[serialString[2].toInt()], memory_map[serialString[2].toInt()]);
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
      for (i = 0; i < c + 1; i++) {
        Serial.print(i);
        Serial.print((char)9);
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

    else if (serialString[1].equalsIgnoreCase("mode")) {
      if (WiFi.getMode() == WIFI_STA) {
        Serial.println("Station");
      }
      else if (WiFi.getMode() == WIFI_AP) {
        Serial.println("Access point");
      }
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
  else if (serialString[0].equalsIgnoreCase("interface")) {
    interface = serialString[1].toInt();
  }
  else if (serialString[0].equalsIgnoreCase("demo")) {
    interface = commandInter;
    demoSectors();
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
  int i, c, a;
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
  }
}



void reportStarting(String comment) {
#if DEBUG
  Serial.println("--------------------------------");
  Serial.print(comment + "...\n");
  execution_time = micros();
  serial_reporter = 0;
#endif
}
void reportEnding() {
#if DEBUG
  Serial.print("\nDone ");
  Serial.print(micros() - execution_time);
  Serial.println("\n--------------------------------\n");
#endif
}
void reportStep() {
#if DEBUG
  Serial.print(serial_reporter);
  Serial.print(", ");
  serial_reporter++;
#endif
}
