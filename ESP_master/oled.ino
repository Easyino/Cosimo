/**
 * @brief It saves an element on the oled buffer
 * 
 * @param alignment left, right, centre
 * @param x Coordinates
 * @param y Coordinates
 * @param limit The x value where the string won't passes
 * @param data Stings
 */
void newDisplayElement(byte alignment, byte x, byte y, byte limit, String data) {
  element[element_counter].limit = limit;
  element[element_counter].aligned = alignment;
  element[element_counter].x = x;
  element[element_counter].y = y;
  element[element_counter].data = data;
  element[element_counter].title = false;
  element_counter++;
  oled_updated = true;
}

void newDisplayElement(byte alignment, byte x, byte y, String data) {
  newDisplayElement(alignment, x, y, 0, data);
}

void updateDisplayElement(byte number, String new_data) {
  if (element[number].data != new_data) {
    element[number].data = new_data;
    oled_updated = true;
  }
}

/**
 * @brief Create a geometrical element on the display
 * 
 * @param x Coordinates
 * @param y Coordinates
 * @param width Dimensions
 * @param height Dimensions
 * @param type rect, rectangle, filled rectangle, circle, filled circle
 */
void newDisplaySpecial(byte x, byte y, byte width, byte height, byte type) {
  special_element[special_element_counter].x = x;
  special_element[special_element_counter].y = y;
  special_element[special_element_counter].width = width;
  special_element[special_element_counter].height = height;
  special_element[special_element_counter].type = type;
  special_element_counter++;
  oled_updated = true;
}

void updateDiplsaySpecial(byte number, byte x, byte y) {
  if (special_element[number].x != x || special_element[number].y != y) {
    special_element[number].x = x;
    special_element[number].y = y;
    oled_updated = true;
  }
}

/**
 * @brief Print every elements on the screen
 * 
 */
void loadDisplay() {
  display.clear();
  for (int i = 0; i < element_counter; i++) {
    if (element[i].aligned != element[i - 1].aligned || i == 0) {
      if (element[i].aligned == left) {
        display.setTextAlignment(TEXT_ALIGN_LEFT);
      }
      else if (element[i].aligned == right) {
        display.setTextAlignment(TEXT_ALIGN_RIGHT);
      }
      else if (element[i].aligned == center) {
        display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
      }
    }
    if (element[i].title != element[i - 1].title || i == 0) {
      if (element[i].title) {
        display.setFont(ArialMT_Plain_16);
      }
      else {
        display.setFont(ArialMT_Plain_10);
      }
    }
    if (element[i].limit == 0) {
      display.drawString(element[i].x, element[i].y, element[i].data);
    }
    else {
      display.drawStringMaxWidth(element[i].x, element[i].y, element[i].limit, element[i].data);
    }
  }
  for (i = 0; i < special_element_counter; i++) {
    switch (special_element[i].type) {
      case rect: {
          display.drawLine(special_element[i].x, special_element[i].y, special_element[i].width, special_element[i].height);
          break;
        }
      case circle: {
          display.drawCircle(special_element[i].x, special_element[i].y, special_element[i].width);
          break;
        }
      case filledCircle: {
          display.fillCircle(special_element[i].x, special_element[i].y, special_element[i].width);
          break;
        }
      case rectangle: {
          display.drawRect(special_element[i].x, special_element[i].y, special_element[i].width, special_element[i].height);
          break;
        }
      case filledRectangle: {
          display.fillRect(special_element[i].x, special_element[i].y, special_element[i].width, special_element[i].height);
          break;
        }
    }
  }
  display.display();
}

/**
 * @brief It select the interface corresponding on (int)interface
 * 
 */
void interfaceSelector() {
  if (interface != loaded_interface) {
    previous_interface = loaded_interface;
    element_counter = 0;
    special_element_counter = 0;
    Serial.print("New interface: ");
    Serial.println(interface);
  }
  switch (interface) {
    case pinInter: {
        pin();
        break;
      }
    case firstConfigInter: {
        firstConfiguration();
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
        question();
        break;
      }
    case commandInter: {
        commandSelection();
        break;
      }
    case settingsInter: {
        settings();
      }
    case menuInter: {
        menu();
        break;
      }
    case wifiCreateInter: {
        wifiCreateDisplay();
        break;
      }
  }
  if (oled_updated) {// Only i fthere is something different it Will print elements on the screen
    oled_updated = false;
    loadDisplay();
  }
}

String temporaneous_pin;
void pin() {
  String message;
  if (interface != loaded_interface) {
    loaded_interface = interface;
    temporaneous_pin = "_";
    d = 0;
    e = 0;
    message = "There are   more tryes before erasing everything";
    f = EEPROM.read(1);
    message[10] = char(48 + (chances - f));
    if (f == 0) {
      newDisplayElement(center, 64, 15, 128, "Insert the pin");
    }
    else if (dialog_interface) {
      newDisplayElement(center, 64, 15, 128, "Chose your own pin and press twice to confirm");
    }
    else {
      newDisplayElement(center, 64, 15, 128, message);
    }
    newDisplayElement(center, 64, 45, temporaneous_pin);
    element[element_counter - 1].title = true;
  }

  if (buttonPressed == up) {
    e = (e + 1) % 10;
    temporaneous_pin[d] = char(48 + e);
  }
  else if (buttonPressed == down) {
    e = (e + 9) % 10;
    temporaneous_pin[d] = char(48 + e);
  }
  else if (buttonPressed == confirm) {
    if (temporaneous_pin[d] == '_' || d == 14) {
      String data;
      for (q = 0; q < d; q += 2) {
        data += temporaneous_pin[q];
      }
      data += '\0';
      Serial.print("Inserted key = ");
      Serial.println(data);
      setMasterKey(data);
      if (!dialog_interface) {
        if (f >= chances) {
          eepromClear();
          ESP.restart();
        }
        else {
          EEPROM.write(1, f);
          EEPROM.commit();
          temporaneous_pin = "_";
          d = 0;
          message = "There are   more tryes before erasing everything";
          message[10] = (char)(48 + (chances - f));
          updateDisplayElement(0, message);
          oled_updated = false;
          wrong_key = false;
        }
        f++;
        return;
      }
      else {
        EEPROM.write(1, 0);
        EEPROM.commit();
        interface = timeInter;
      }
    }
    else {
      temporaneous_pin += " _";
      d += 2;
    }
    e = 0;
  }
  updateDisplayElement(1, temporaneous_pin);
}

void firstConfiguration() { 
}

void timeTrack() {
  if (interface != loaded_interface) {
    loaded_interface = interface;
    newDisplayElement(left, 1, 1, String (millis() / 1000));
    newDisplayElement(right, 128, 52, wifi_IP);
  }
  if (ota_initialised) {
    updateDisplayElement(1, "OTA " + wifi_IP);
  }
  updateDisplayElement(0, String (millis() / 1000));
}

#define n_rows 5
int element_selected;
/**
 * @brief Managmnt of commands and items on a list
 * 
 * @return The selected item with confirm button
 */
int elementListSelector() {
  if (buttonPressed == up) {
    if (element_selected != 0) {
      element_selected--;
      if (element_selected % n_rows == n_rows - 1) {
        updateList();
      }
      updateDiplsaySpecial(0, element[element_selected % n_rows].x - 6, element[element_selected % n_rows].y + 6);
    }
  }
  else if (buttonPressed == down) {
    if (elements_list[element_selected + 1] != "") {
      element_selected++;
      if (element_selected % n_rows == 0) {
        updateList();
      }
      updateDiplsaySpecial(0, element[element_selected % n_rows].x - 6, element[element_selected % n_rows].y + 6);
    }
  }
  else if (buttonPressed == confirm) {
    return element_selected;
  }
  return -1;
}



void createList(byte offset_x, byte offset_y, bool selector) {
  element_selected = 0;
  if (selector) {
    newDisplaySpecial(offset_x + 4, offset_y + 6, 3, 0, circle);
    offset_x += 10;
  }
  for (d = 0; d < n_rows; d++) {
    newDisplayElement(left, offset_x, d * 64 / n_rows + offset_y, elements_list[d]);
  }
}
void createList(byte offset, bool selector) {
  createList(offset, 0, selector);
}


void updateList() {
  for (d = 0; d < n_rows; d++) {
    updateDisplayElement(d, elements_list[element_selected - (element_selected % n_rows) + d]);
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
    createList(1, false);
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
    loadTitles();
    createList(0, true);
  }
  com = elementListSelector();
  if (com != -1) {
    loadSector(com + 2);
    //sendSlave(memory_map[0], memory_type[0]); /// To try i2c
  }
}

/////////////////////////////////////////////////////////// Cooming soon
const char* dialogText[] PROGMEM = {
  "Someone wants to connect. Do you want it?",
  "Do you want to erase everything?"
};

void question() {
  if (interface != loaded_interface) {
    loaded_interface = interface;
    newDisplayElement(left, 1, 1, 90, dialogText[dialog_interface]);
    clearList();
    elements_list[0] = "No";
    elements_list[1] = "Yes";
    createList(100, 15, true);
  }
  if (elementListSelector() != -1){
    dialog_interface = element_selected;
    interface = previous_interface;
  }
}

void menu() {
  if (interface != loaded_interface) {
    loaded_interface = interface;
    createList(0, true);
  }
  if (elementListSelector() != -1) {
    Serial.print("selected element n.: ");
    Serial.println(element_selected);
  }
}

void settings() {

}

void wifiCreateDisplay() {
  if (interface != loaded_interface) {
    loaded_interface = interface;
    newDisplayElement(left, 1, 1, "Connect your device to:");
    newDisplayElement(left, 1, 15, "Easyino Cosimo");
    newDisplayElement(left, 1, 52, "go to: 10.10.10.1");
  }
}
