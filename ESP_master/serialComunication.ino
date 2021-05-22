void executeSerialCommands() {
  if (serialString[0].equalsIgnoreCase("info")) {
    reportStarting("Printing info");

    Serial.println("\n\nCheckpoints:");
    for (i = 0; checkpoint_memory[i] != 0; i++) {
      Serial.print(checkpoint_memory[i]);
      Serial.print(", ");
    }

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
    for(i = 0; i < element_counter; i++){
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
      EEPROM.write(0, serialString[2].toInt());
      EEPROM.commit();
      Serial.print("setting wifi mode to ");
      Serial.println(serialString[2].toInt());
    }
    else if(serialString[1].equalsIgnoreCase("debug")){
      #define DO(x...) Serial.println(F( #x )); x; break
      switch (serialString[2][0]) {
        case 'F': DO(WiFiOff());
        case 'N': DO(WiFiOn());
        case '1': DO(WiFi.mode(WIFI_AP));
        case '2': DO(WiFi.mode(WIFI_AP_STA));
        case '3': DO(WiFi.mode(WIFI_STA));
        case 'R': DO(if (((GPI >> 16) & 0xf) == 1) ESP.reset() /* else must hard reset */);
        case 'd': DO(WiFi.disconnect());
        case 'b': DO(WiFi.begin());
        case 'B': DO(WiFi.begin(ext_ssid, ext_password));
        case 'r': DO(WiFi.reconnect());
        case 'c': DO(wifi_station_connect());
        case 'a': DO(WiFi.setAutoReconnect(false));
        case 'A': DO(WiFi.setAutoReconnect(true));
        case 'n': DO(WiFi.setSleepMode(WIFI_NONE_SLEEP));
        case 'l': DO(WiFi.setSleepMode(WIFI_LIGHT_SLEEP));
        case 'm': DO(WiFi.setSleepMode(WIFI_MODEM_SLEEP));
        case 'S': DO(WiFi.config(local_ip, gateway, subnet)); // use static address
        case 's': DO(WiFi.config(0u, 0u, 0u));                // back to dhcp client
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
  else if (serialString[0].equalsIgnoreCase("interface")){
    interface = serialString[1].toInt();
  }
  else if (serialString[0].equalsIgnoreCase("demo")){
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
  Serial.println("--------------------------------");
  Serial.print(comment + "...\n");
  execution_time = micros();
  serial_reporter = 0;
}
void reportEnding() {
  Serial.print("\nDone ");
  Serial.print(micros() - execution_time);
  Serial.println("\n--------------------------------\n");
}
void reportStep() {
  Serial.print(serial_reporter);
  Serial.print(", ");
  serial_reporter++;
}
