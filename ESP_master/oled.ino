void newDisplayElement(int x, int y, String data) {
  newDisplayElement(left, x, y, data);
}

void newDisplayElement(int alignment, int x, int y, String data) {
  element[element_counter].aligned = alignment;
  element[element_counter].x = x;
  element[element_counter].y = y;
  element[element_counter].data = data;
  element_counter++;
  loadDisplay();
}

void updateDisplayElement(int number, String new_data) {
  if (element[number].data != new_data) {
    element[number].data = new_data;
    loadDisplay();
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

String temporaneous_pin;
void pin() {
  if (interface != loaded_interface) {
    loaded_interface = interface;
    element_counter = 0;
    temporaneous_pin = "_";
    a = 0;
    i = 0;
    if (EEPROM.read(1) == 0) {
      newDisplayElement(center, 64, 10, "Insert the pin");
    }
    else {
      newDisplayElement(center, 64, 10, "There are" + char(48 + 5 - EEPROM.read(1) + "more tryes before formatting everything"));
    }
    newDisplayElement(center, 64, 40, temporaneous_pin);
  }

  if (debouncedButtons == up) {
    i = (i + 1) % 10;
    temporaneous_pin[a * 2] = char(48 + i);
  }
  else if (debouncedButtons == down) {
    i = (i + 9) % 10;
    temporaneous_pin[a * 2] = char(48 + i);
  }
  else if (debouncedButtons == confirm) {
    if (temporaneous_pin[a * 2] == '_' || a == 16) {
      String data;
      for (r = 0; temporaneous_pin[r] != '\0'; r += 2) {
        data += temporaneous_pin[r];
      }
      setMasterKey(data);
    }
    else {
      temporaneous_pin += " _";
      a += 2;
    }
  }
  updateDisplayElement(1, temporaneous_pin);
}
