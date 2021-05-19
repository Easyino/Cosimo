/**
   @brief It saves an element on the oled buffer

   @param alignment left, right, centre
   @param x Coordinates
   @param y Coordinates
   @param limit The x value where the string won't passes
   @param data Stings
*/
void newDisplayElement(byte alignment, byte x, byte y, byte limit, String data) {
  element[element_counter].limit = limit;
  element[element_counter].aligned = alignment;
  element[element_counter].x = x;
  element[element_counter].y = y;
  element[element_counter].data = data;
  element[element_counter].font = 0;
  element_counter++;
  oled_updated = true;
}

void newDisplayElement(byte alignment, byte x, byte y, String data) {
  newDisplayElement(alignment, x, y, 0, data);
}

void DEposition(byte number, byte x, byte y) {
  if (element[number].x != x || element[number].y != y) {
    element[number].x = x;
    element[number].y = y;
    oled_updated = true;
  }
}
void DEdata(byte number, String data) {
  if (element[number].data != data) {
    element[number].data = data;
    oled_updated = true;
  }
}
void DEalignment(byte number, byte alignment) {
  if (element[number].aligned != alignment) {
    element[number].aligned = alignment;
    oled_updated = true;
  }
}
void DElimit(byte number, byte limit) {
  if (element[number].limit != limit) {
    element[number].limit = limit;
    oled_updated = true;
  }
}
void DEfont(byte number, byte font) {
  if (element[number].font != font) {
    element[number].font = font;
    oled_updated = true;
  }
}



/**
   @brief Create a geometrical element on the display

   @param x Coordinates
   @param y Coordinates
   @param width Dimensions
   @param height Dimensions
   @param type rect, rectangle, filled rectangle, circle, filled circle
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

void newDisplayIcon(byte x, byte y, byte ref) {
  icon_element[icon_element_counter].x = x;
  icon_element[icon_element_counter].y = y;
  icon_element[icon_element_counter].ref = ref;
  icon_element_counter++;
  oled_updated = true;
}

void DSposition(int number, int x, int y) {
  if (special_element[number].x != x || special_element[number].y != y) {
    special_element[number].x = x;
    special_element[number].y = y;
    oled_updated = true;
  }
}

void DSshape(int number, int width, int height) {
  if (special_element[number].width != width || special_element[number].height != height) {
    special_element[number].width = width;
    special_element[number].height = height;
    oled_updated = true;
  }
}

void DStype(int number, int type) {
  if (special_element[number].type != type) {
    special_element[number].type = type;
    oled_updated = true;
  }
}

/**
   @brief Print every elements on the screen

*/
void loadDisplay() {
  previousButton = buttonPressed;
  buttonPressed = debouncedButtons();
  if (previousButton != buttonPressed) {
    previousButton = buttonPressed;
    triggButton = buttonPressed;
  }
  else if (triggButton != -1) {
    triggButton = -1;
  }
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
        display.setTextAlignment(TEXT_ALIGN_CENTER);
      }
    }
    if (element[i].font != element[i - 1].font || i == 0) {
      if (element[i].font == 2) {
        display.setFont(ArialMT_Plain_24);
      }
      else if (element[i].font == 1) {
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
   @brief It select the interface corresponding on (int)interface

*/
void interfaceSelector() {
  previousButton = buttonPressed;
  buttonPressed = debouncedButtons();
  if (previousButton != buttonPressed) {
    previousButton = buttonPressed;
    triggButton = buttonPressed;
  }
  else if (triggButton != -1) {
    triggButton = -1;
  }

  if (interface != loaded_interface) {
    if (loaded_interface < questionInter){
      previous_interface = loaded_interface;
    }
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
    case wifiCreateInter: {
        wifiCreateDisplay();
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
    case displayInter: {
        setDisplay();
        break;
      }
    case menuInter: {
        menu();
        break;
      }
    case commandInter: {
        commandSelection();
        break;
      }
    case wifiInter: {
        wifi();
        break;
      }
    case settingsInter: {
        settings();
        break;
      }
    default:{
        question();
        break;
    }
  }
  if (oled_updated) {// Only i fthere is something different it Will print elements on the screen
    oled_updated = false;
    loadDisplay();
  }
}


#define n_rows 5
int element_selected;
int sel;
/**
   @brief Managmnt of commands and items on a list

   @return The selected item with confirm button
*/

void createList(byte offset_x, byte offset_y, bool selector) {
  if (selector) {
    newDisplaySpecial(offset_x + 4, offset_y + 6, 3, 0, circle);
    offset_x += 10;
  }
  begin_list = element_counter;
  for (d = 0; d < n_rows; d++) {
    newDisplayElement(left, offset_x, d * 64 / n_rows + offset_y, elements_list[d]);
  }

  if (title_list) {
    newDisplaySpecial(0, 64 / n_rows, 128, 64 / n_rows, rect);
    if (element[begin_list + 1].data != ""){
      element_selected = 1;
      DSposition(0, element[element_selected % n_rows + begin_list].x - 6, element[element_selected % n_rows + begin_list].y + 6);
    }
    else {
      DStype(0, rectangle);
    }
  }
  else {
    element_selected = 0;
  }
}
void createList(byte offset, bool selector) {
  createList(offset, 0, selector);
}


void updateList() {
  if (title_list) {
    if (element_selected < n_rows) {
      DSposition(1, 0, 64 / n_rows);
      DSshape(1, 128, 64 / n_rows);
    }
    else {
      DSposition(1, 0, -1);
      DSshape(1, 128, -1);
    }
  }

  for (d = 0; d < n_rows; d++) {
    DEdata(d + begin_list, elements_list[element_selected - (element_selected % n_rows) + d]);
  }
}

void setParameter(int address, int min, int max){
  parameter.address = address;
  parameter.data = EEPROM.read(address);
  parameter.min = min;
  parameter.max = max;
  newDisplayElement(right, 128, element[element_selected % n_rows + begin_list].y, String(parameter.data));
}

void updateParameter(){
  EEPROM.write(parameter.address, parameter.data);
  EEPROM.commit();
  parameter.data = -1;
  element_counter--;
}

void clearList() {
  title_list = false;
  for (d = 0; elements_list[d] != ""; d++) {
    elements_list[d] = "";
  }
}

int elementListSelector() { //// Funzione del boss del poppin che gestisce praticamente tutta l'interfaccia
  if (triggButton == up) {
    if (parameter.data == -1){
      if (element_selected != 0) {
        element_selected--;
        if (element_selected % n_rows == n_rows - 1) {
          updateList();
        }
        DSposition(0, element[element_selected % n_rows + begin_list].x - 6, element[element_selected % n_rows + begin_list].y + 6);
        if (element_selected == 0 && title_list) {
          DStype(0, rectangle);
        }
        else {
          DStype(0, circle);
        }
      }
    }
    else if (parameter.data < parameter.max){
      parameter.data++;
      DEdata(element_counter - 1, String(parameter.data));
    }

  }
  else if (triggButton == down) {
    if (parameter.data == -1){
      if (elements_list[element_selected + 1] != "") {
        element_selected++;
        if (element_selected % n_rows == 0) {
          updateList();
        }
        DSposition(0, element[element_selected % n_rows + begin_list].x - 6, element[element_selected % n_rows + begin_list].y + 6);
        DStype(0, circle);
      }
    }
    else if (parameter.data > parameter.min){
      parameter.data--;
      DEdata(element_counter - 1, String(parameter.data));
    }
  }
  else if (triggButton == confirm) {
    if (parameter.data == -1){
      if (element_selected != 0) {
        DStype(0, filledCircle);
      }
      return element_selected;
    }
    else {
      updateParameter();
      DStype(0, circle);
    }
  }
  return -1;
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
    DEfont(element_counter - 1, 1);
  }

  if (triggButton == up) {
    e = (e + 1) % 10;
    temporaneous_pin[d] = char(48 + e);
  }
  else if (triggButton == down) {
    e = (e + 9) % 10;
    temporaneous_pin[d] = char(48 + e);
  }
  else if (triggButton == confirm) {
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
          DEdata(0, message);
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
  DEdata(1, temporaneous_pin);
}

void firstConfiguration() {
 //////////
}


////////////////////////////////////////////////////////////////////////////////// Probably thease function will be removed in the future
void wifiCreateDisplay() {
  if (interface != loaded_interface) {
    loaded_interface = interface;
    newDisplayElement(left, 1, 1, "Connect your device to:");
    newDisplayElement(left, 1, 15, "Easyino Cosimo");
    newDisplayElement(left, 1, 52, "go to: 10.10.10.1");
  }
}

void timeTrack() {
  if (interface != loaded_interface) {
    loaded_interface = interface;
    newDisplayElement(left, 1, 1, String (millis() / 1000));
    newDisplayElement(right, 128, 52, wifi_IP);
  }
  if (ota_initialised) {
    DEdata(1, "OTA " + wifi_IP);
  }
  DEdata(0, String (millis() / 1000));
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
//////////////////////////////////////////////////////////////////////////////////


const char* dialogText[] PROGMEM = {
  "Do you want to change your password?",
  "Do you want to erase everything?",
  "Someone wants to connect. Do you want it?"
};
const int questionLink[] PROGMEM = {
  pinInter,
  back,
  back
};
enum select_questions {
  Qpassword = questionInter,
  Qreset,
  Qconnect
};

void question() {
  if (interface != loaded_interface) {
    loaded_interface = interface;
    newDisplayElement(left, 1, 1, 90, dialogText[interface - questionInter]);
    clearList();
    elements_list[0] = "No";
    elements_list[1] = "Yes";
    createList(100, 15, true);
  }
  sel = elementListSelector();
  if (sel != -1) {
    if (sel){
      if (questionLink[interface - questionInter] == back){
        switch (interface){
          case Qreset:{
            eepromClear();
            ESP.restart();
            break;
          }
          case Qconnect:{
            /// Non so longhino come volevi fare
            Serial.println("Ciao mona");
            break;
          }
        }
        interface = previous_interface;
      }
      else {
        interface = questionLink[interface - questionInter];
      }
    }
    else {
      interface = previous_interface;
    }
  }
}

void menu() {
  if (interface != loaded_interface) {
    loaded_interface = interface;
    clearList();
    elements_list[0] = "Commands";
    elements_list[1] = "WiFi";
    elements_list[2] = "Settings";
    createList(0, 64 * 1.25 / n_rows, true);
  }
  if (elementListSelector() != -1) {
    interface = element_selected + commandInter;
  }
}

void commandSelection() {
  if (interface != loaded_interface) {
    loaded_interface = interface;
    clearList();
    title_list = true;
    elements_list[0] = "COMMANDS";
    loadTitles();
    createList(0, true);
  }
  sel = elementListSelector();
  if (sel != -1) {
    if (sel != 0) {
      loadSector(sel + 1);
      //sendSlave(memory_map[0], memory_type[0]); /// To try i2c
    }
    else {
      interface = menuInter;
    }
  }
}

void wifi() {
  if (interface != loaded_interface) {
    loaded_interface = interface;
    clearList();
    title_list = true;
    elements_list[0] = "WIFI";
    elements_list[1] = "Add";
    elements_list[2] = "Saved";
    createList(0, true);
  }
  sel = elementListSelector();
  if (sel != -1) {
    if (sel != 0) {

    }
    else {
      interface = menuInter;
    }
  }
}
void settings() {
  if (interface != loaded_interface) {
    loaded_interface = interface;
    clearList();
    title_list = true;
    elements_list[0] = "SETTINGS";
    elements_list[1] = "Password";
    elements_list[2] = "Display";
    elements_list[3] = "Reset";
    createList(0, true);
  }
  sel = elementListSelector();
  if (sel != -1) {
    if (sel != 0) {
      if (sel == 1){
        interface = Qpassword;
      }
      else if (sel == 2){
        interface = displayInter;
      }
      else if (sel == 3){
        interface = Qreset;
      }
    }
    else {
      interface = menuInter;
    }
  }
}

void setDisplay(){
  if (interface != loaded_interface) {
    loaded_interface = interface;
    clearList();
    title_list = true;
    elements_list[0] = "DISPLAY";
    elements_list[1] = "Flip";
    elements_list[2] = "Contrast";
    createList(0, true);
  }
  sel = elementListSelector();
  if (sel != -1) {
    if (sel != 0) {
      if (sel == 1){
        setParameter(3, 0, 1);
      }
      else if (sel == 2){
        setParameter(2, 0, 4);
      }
    }
    else {
      interface = settingsInter;
    }
  }
}
