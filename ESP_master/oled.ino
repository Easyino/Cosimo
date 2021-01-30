void newDisplayElement(int x, int y, String data) {
  newDisplayElement(left, x, y, data);
}

void newDisplayElement(int alignment, int x, int y, String data) {
  element[element_counter].aligned = alignment;
  element[element_counter].x = x;
  element[element_counter].y = y;
  element[element_counter].data = data;
  element_counter++;
  oled_updated = true;
}

void updateDisplayElement(int number, String new_data) {
  if (element[number].data != new_data) {
    element[number].data = new_data;
    oled_updated = true;
  }
}

void loadDisplay() {
  display.clear();
  for (int i = 0; i < element_counter; i++) {
    if (element[i].aligned == left) {
      display.setTextAlignment(TEXT_ALIGN_LEFT);
    }
    else if (element[i].aligned == right) {
      display.setTextAlignment(TEXT_ALIGN_RIGHT);
    }
    else if (element[i].aligned == center) {
      display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
    }
    display.drawString(element[i].x, element[i].y, element[i].data);
  }
  display.display();
}


void interfaceSelector() {
  if (interface != loaded_interface) {
    previous_interface = loaded_interface;
    element_counter = 0;
  }
  switch (interface) {
    case pinInter: {
        pin(false);
        break;
      }
    case firstPinInter: {
        pin(true);
        break;
      }
    case timeInter: {
        timeTrack();
        break;
      }
    case logInter: {
        logInterface();
        break;
      }
    case questionInter: {
        //question();
        break;
      }
    case commandInter: {
        commandSelection();
        break;
      }
  }
  if (oled_updated) {
    oled_updated = false;
    loadDisplay();
  }
}


String temporaneous_pin;
void pin(bool first_configuration) {
  String message;
  if (interface != loaded_interface) {
    loaded_interface = interface;
    temporaneous_pin = "_";
    d = 0;
    e = 0;
    message = "There are   more tryes\nbefore erasing everything";
    f = EEPROM.read(1);
    message[10] = char(48 + (chances - f + 1));
    if (f == 0) {
      newDisplayElement(center, 64, 20, "Insert the pin");
    }
    else if (first_configuration) {
      newDisplayElement(center, 64, 20, "Chose your own pin");
    }
    else {
      newDisplayElement(center, 64, 20, message);
    }
    newDisplayElement(center, 64, 45, temporaneous_pin);
  }

  if (debouncedButtons() == up) {
    e = (e + 1) % 10;
    temporaneous_pin[d] = char(48 + e);
  }
  else if (debouncedButtons() == down) {
    e = (e + 9) % 10;
    temporaneous_pin[d] = char(48 + e);
  }
  else if (debouncedButtons() == confirm) {
    if (temporaneous_pin[d] == '_' || d == 16) {
      String data;
      for (q = 0; temporaneous_pin[q] != '_'; q += 2) {
        data += temporaneous_pin[q];
      }
      data += '\0';
      Serial.print("Inserted key = ");
      Serial.println(data);
      setMasterKey(data);
      if (!first_configuration) {
        if (f > chances) {
          eepromClear();
          ESP.restart();
        }
        else if (f != 0) {
          EEPROM.write(1, f);
          EEPROM.commit();
          temporaneous_pin = "_";
          d = 0;
          message = "There are   more tryes\nbefore erasing everything";// Si cancella da sola la stringa...
          message[10] = (char)(48 + (chances - f + 1));
          updateDisplayElement(0, message);
          oled_updated = false;
          wrong_key = false;
        }
        f++;
        while (debouncedButtons() == confirm);
        return;
      }
      else {
        EEPROM.write(1, 0);
        EEPROM.commit();
        interface = 2;
      }
    }
    else {
      temporaneous_pin += " _";
      d += 2;
    }
    e = 0;
    while (debouncedButtons() == confirm);
  }
  updateDisplayElement(1, temporaneous_pin);
}

void timeTrack() {
  if (interface != loaded_interface) {
    loaded_interface = interface;
    newDisplayElement(1, 1, String (millis() / 1000));
    newDisplayElement(right, 128, 52, wifi_IP);
  }
  if (ota_initialised) {
    updateDisplayElement(1, "OTA " + wifi_IP);
  }
  updateDisplayElement(0, String (millis() / 1000));
}

#define n_rows 5
int element_selected;
int elementListSelector() {
  if (debouncedButtons() == up) {
    if (element_selected != 0) {
      element_selected--;
      if (element_selected % n_rows == 0) {
        updateList();
      }
    }
  }
  else if (debouncedButtons() == down) {
    if (elements_list[element_selected + 1] != "") {
      element_selected++;
      if (element_selected % n_rows == 0) {
        updateList();
      }
    }
  }
  else if (debouncedButtons() == confirm) {
    return element_selected;
  }
  return -1;
}

void createList() {
  element_selected = 0;
  for (d = 0; d < n_rows; d++) {
    newDisplayElement(1, d * (64 / n_rows) + 1, elements_list[d]);
  }
}

void updateList() {
  for (d = 0; d < n_rows; d++) {
    updateDisplayElement(d, elements_list[element_selected / n_rows + d]);
  }
}

void clearList() {
  for (d = 0; elements_list[d] != ""; d++) {
    elements_list[d] = "";
  }
}

void logInterface() {
  if (interface != loaded_interface) {
    loaded_interface = interface;
    clearList();
    createList();
  }
}

void oledReport(String data) {
  if (interface == logInter) {
    for (int i = n_rows; i >= 0; i--) {
      elements_list[i + 1] = elements_list[i];
    }
    elements_list[0] = data;
    updateList();
  }
}

void commandSelection() {
  int com;
  if (interface != loaded_interface) {
    loaded_interface = interface;
    clearList();
    loadTitles();// Not working for now
    createList();
  }
  com = elementListSelector();
  if (com != -1){
    loadSector(com);
  }
}

/////////////////////////////////////////////////////////// Cooming soon
//const char* dialogText[] PROGMEM = {
//  "Someone wants to connect. Do you want it?",
//  "Do you want to erase everything?"
//};
//
//void question() {
//  if (interface != loaded_interface) {
//    loaded_interface = interface;
//    newDisplayElement();
//    newDisplayElement();
//    newDisplayElement();
//  }
//}
