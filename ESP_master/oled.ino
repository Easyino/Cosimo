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

void newDisplayIcon(byte x, byte y, byte ref) {
  icon_element[icon_element_counter].x = x;
  icon_element[icon_element_counter].y = y;
  icon_element[icon_element_counter].ref = ref;
  icon_element_counter++;
  oled_updated = true;
}

void DInumber(int number, int ref){
  if (icon_element[number].ref != ref) {
    icon_element[number].ref = ref;
    oled_updated = true;
  }
}

void DIposition(int number, int x, int y){
  if (icon_element[number].x != x || icon_element[number].y != y) {
    icon_element[number].x = x;
    icon_element[number].y = y;
    oled_updated = true;
  }
}

/**
   @brief Print every elements on the screen

*/
void loadDisplay() {
  int i;
  display.clear();

  for(i = icon_element_counter - 1; i >= 0 ; i--){
    if (icon_element[i].ref != invisible){
      display.drawIco16x16(icon_element[i].x, icon_element[i].y, icons[icon_element[i].ref], false);
    }
  }
  for (i = 0; i < element_counter; i++) {
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
  millis_time = millis();
  if (timer_need){
    checkTimers();
  }

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
    if (history_interface[previous_interface] != interface && interface < questionInter){
      previous_interface = (previous_interface + 1) % 5;
      history_interface[previous_interface] = interface;
    }
    //Clearing flags and counters
    element_counter = 0;
    special_element_counter = 0;
    icon_element_counter = 0;
    timer_need = 0;
    details_list = 0;
    Serial.print("Updated interface: ");
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
    case scanWifiInter: {
      scanWifi();
      break;
    }
    case savedWifiInter: {
      savedWifi();
      break;
    }
    case settingsInter: {
      settings();
      break;
    }
    case commSettingsInter: {
      setCommand();
      break;
    }
    case displayInter: {
      setDisplay();
      break;
    }
    case buttonsInter: {
      setButtons();
      break;
    }
    default:{
      question();
      break;
    }
  }
  if (oled_updated) {// Only if there is something different it Will print elements on the screen
    oled_updated = false;
    loadDisplay();
  }
}
void interfaceBack(){
  if (interface < questionInter){
    previous_interface = (previous_interface + 4) % 5;
  }
  interface = history_interface[previous_interface];
}


int element_selected;
int sel;
/**
   @brief Managmnt of commands and items on a list

   @return The selected item with confirm button
*/

void createList(byte offset_x, byte offset_y, bool selector) {
  int d;
  if (selector) {
    newDisplaySpecial(offset_x + 4, offset_y + 6, 3, 0, circle);
    offset_x += 10;
  }
  begin_list = element_counter;
  for (d = 0; d < n_rows; d++) {
    newDisplayElement(left, offset_x, d * 64 / n_rows + offset_y, elements_list[d]);
  }
  if (details_list){
    for (d = 0; d < n_rows; d++){
      if (details_elements_list[d].type == txt){
        newDisplayElement(right, 128, d * 64 / n_rows + offset_y, String(details_elements_list[d].data));
      }
      else {
        newDisplayIcon(128 - 16, d * 64 / n_rows + offset_y - 5, details_elements_list[d].data);
      }
    }
    
  }
  element_selected = 0;
  if (title_list) {
    newDisplaySpecial(0, 64 / n_rows, 128, 64 / n_rows, rect);
    if (details_list){
      details_elements_list[0].type = invisible;
    }
    if (element[begin_list + 1].data != ""){
      element_selected = 1;
      DSposition(0, element[element_selected % n_rows + begin_list].x - 6, element[element_selected % n_rows + begin_list].y + 6);
    }
    else {
      DStype(0, rectangle);
    }
  }
}
void createList(byte offset, bool selector) {
  createList(offset, 0, selector);
}


void updateList() {
  int d;
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
    if (details_list){
      if (details_elements_list[d].type == txt){
        DEdata(d + begin_list + n_rows, String(details_elements_list[element_selected - (element_selected % n_rows) + d].data));
      }
      else {
        DInumber(d, details_elements_list[element_selected - (element_selected % n_rows) + d].data);/// to add icon specific begin_list
      }
    }
  }
}

// position on the EEPROM, minimum value, maximum value
void setParameter(int address, int min, int max){
  parameter.address = address;
  parameter.data = EEPROM.read(address);
  parameter.min = min;
  parameter.max = max;
  if (max == 1){
    newDisplayElement(right, 128, element[element_selected % n_rows + begin_list].y, parameter.data ? "ON" : "OFF");
  }
  else {
    newDisplayElement(right, 128, element[element_selected % n_rows + begin_list].y, String(parameter.data));
  }
}

void updateParameter(){
  EEPROM.write(parameter.address, parameter.data);
  EEPROM.commit();
  parameter.data = -1;
  element_counter--;
  eepromPar(parameter.address);
}

void clearList() {
  int d;
  title_list = false;
  for (d = 0; elements_list[d] != ""; d++) {
    elements_list[d] = "";
    details_elements_list[d].type = image;
    details_elements_list[d].data = invisible;
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
        if (!element_selected && title_list) {
          DStype(0, rectangle);
        }
        else {
          DStype(0, circle);
        }
      }
    }
    else if (parameter.data < parameter.max){
      parameter.data++;
      if (parameter.max == 1){
        DEdata(element_counter - 1, parameter.data ? "ON" : "OFF");
      }
      else {
        DEdata(element_counter - 1, String(parameter.data));
      }
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
      if (parameter.max == 1){
        DEdata(element_counter - 1, parameter.data ? "ON" : "OFF");
      }
      else {
        DEdata(element_counter - 1, String(parameter.data));
      }
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

int d, e, f;
String temporaneous_pin;
void pin() {
  int q;
  String message;
  if (interface != loaded_interface) {
    d = 0;
    e = 0;
    loaded_interface = interface;
    temporaneous_pin = "_";
    message = "There are   more tryes before erasing everything";
    f = EEPROM.read(0);
    message[10] = char(48 + (chances - f));
    if (f == 0) {
      newDisplayElement(center, 64, 15, 128, "Insert the pin");
      Serial.println("No pin inserted");
    }
    else if (dialog_interface) {
      newDisplayElement(center, 64, 15, 128, "Chose your own pin and press twice to confirm");
      Serial.println("Chose your pin");
    }
    else {
      newDisplayElement(center, 64, 15, 128, message);
      Serial.println("Chances");
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
        f++;
        if (f >= chances) {
          eepromClear();
          ESP.restart();
        }
        else {
          EEPROM.write(0, f);
          EEPROM.commit();
          temporaneous_pin = "_";
          d = 0;
          message = "There are   more tryes before erasing everything";
          message[10] = (char)(48 + (chances - f));
          DEdata(0, message);
          oled_updated = false; // To make it look better I update the screen only when the pin is incorrect
          wrong_key = false;
        }
        return;
      }
      else {
        EEPROM.write(0, 0);
        EEPROM.commit();
        interfaceBack();
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
    newDisplayElement(left, 1, 52, "go to: 192.168.4.1");
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
  "Do you want to go back to default settings?",
  "Do you want to erase everything?",
  "Someone wants to connect. Do you want it?",
  "Do you want to delete these WiFi credentials?"
};
const int questionLink[] PROGMEM = {
  back,
  back,
  back,
  back
};
enum select_questions {
  Qdefault = questionInter,
  Qreset,
  Qconnect,
  QeraseWifi
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
          case Qdefault:{
            setDefault();
            for (int i = 2; i < EEPROM_offset; i++){
              eepromPar(i);
            }
            break;
          }
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
          case QeraseWifi:{
            loadSector(1);
            updateCommand(dialog_interface, "", text);
            updateCommand(dialog_interface, "", text);// dialog_interface is not incremented by 1 because the function shift automatically the empty elements
            updateEEPROM();
            break;
          }
        }
        interfaceBack();
      }
      else {
        interface = questionLink[interface - questionInter];
      }
    }
    else {
      interfaceBack();
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
    createList(0, true);
    newDisplayIcon(112, 0, access_point);
    if (wifi_state == STA){
      timer_need = 1;
      goto up_icon;
    }
    else {
      DInumber(0, access_point);
    }
  }
  if (wifi_state == STA){
    if (!(timer_trigg % 2)){
      up_icon:
      int rssi = WiFi.RSSI();
      Serial.print("rssi: ");
      Serial.println(rssi);
      if (rssi > -60){
        DInumber(0, full_wifi);
      }
      else if (rssi > -80){
        DInumber(0, half_wifi);
      }
      else {
        DInumber(0, small_wifi);
      }
    }
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
      sendSector();
    }
    else {
      interfaceBack();
    }
  }
}

void wifi() {
  if (interface != loaded_interface) {
    loaded_interface = interface;
    clearList();
    title_list = true;
    elements_list[0] = "WIFI    " + wifi_IP;
    elements_list[1] = "Access point";
    elements_list[2] = "Connect";
    elements_list[3] = "Scan";
    elements_list[4] = "Saved";
    elements_list[5] = "Connect on start";
    createList(0, true);
  }
  sel = elementListSelector();
  if (sel != -1) {
    if (sel != 0) {
      if (sel == 1) {
        createNetwork();
        interface = menuInter;
      }
      else if (sel == 2) {
        tryConnect();
        interface = menuInter;
      }
      else if (sel == 3){
        interface = scanWifiInter;
      }
      else if (sel == 4){
        interface = savedWifiInter;
      }
      else if (sel == 5){
        setParameter(2, 0, 1);
      }
    }
    else {
      interfaceBack();
    }
  }
}

void scanWifi(){
  if (interface != loaded_interface) {
    loaded_interface = interface;
    clearList();
    title_list = true;
    elements_list[0] = "SCAN";
    details_list = true;
    loadScannedWifi();
    createList(0, true);
  }
  sel = elementListSelector();
  if (sel != -1) {
    if (sel != 0) {

    }
    else {
      interfaceBack();
    }
  }
}

void savedWifi(){
  int f;
  if (interface != loaded_interface) {
    loadSector(1);
    loaded_interface = interface;
    clearList();
    title_list = true;
    elements_list[0] = "SAVED";
    for (f = 0; memory_map[f] != ""; f += 2){
      elements_list[f / 2 + 1] = memory_map[f];
    }
    createList(0, true);
  }
  sel = elementListSelector();
  if (sel != -1) {
    if (sel != 0) {
      if (memory_map[2] != ""){
        dialog_interface = (sel - 1) * 2;
        interface = QeraseWifi;
      }
    }
    else {
      interfaceBack();
    }
  }
}

void settings() {
  if (interface != loaded_interface) {
    loaded_interface = interface;
    clearList();
    title_list = true;
    elements_list[0] = "SETTINGS";
    elements_list[1] = "Commands";
    elements_list[2] = "Display";
    elements_list[3] = "Buttons";
    elements_list[4] = "Default";
    elements_list[5] = "Reset all";
    createList(0, true);
  }
  sel = elementListSelector();
  if (sel != -1) {
    if (sel != 0) {
      if (sel == 1){
        interface = commSettingsInter;
      }
      else if (sel == 2){
        interface = displayInter;
      }
      else if (sel == 3){
        interface = buttonsInter;
      }
      else if (sel == 4){
        interface = Qdefault;
      }
      else if (sel == 5){
        interface = Qreset;
      }
    }
    else {
      interfaceBack();
    }
  }
}

void setCommand(){
  if (interface != loaded_interface) {
    loaded_interface = interface;
    clearList();
    title_list = true;
    elements_list[0] = "COMMANDS";
    elements_list[1] = "Auto login";
    createList(0, true);
  }
  sel = elementListSelector();
  if (sel != -1) {
    if (sel != 0) {
      if (sel == 1){
        setParameter(8, 0, 1);
      }
    }
    else {
      interfaceBack();
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
    elements_list[3] = "Rows";
    createList(0, true);
  }
  sel = elementListSelector();
  if (sel != -1) {
    if (sel != 0) {
      if (sel == 1){
        setParameter(4, 0, 1);
      }
      else if (sel == 2){
        setParameter(3, 1, 5);
      }
      else if (sel == 3){
        setParameter(5, 4, 7);
      }
    }
    else {
      interfaceBack();
    }
  }
}


void setButtons(){
  if (interface != loaded_interface) {
    loaded_interface = interface;
    clearList();
    title_list = true;
    elements_list[0] = "BUTTONS";
    elements_list[1] = "Bouncing time";
    elements_list[2] = "Scrolling time";
    createList(0, true);
  }
  sel = elementListSelector();
  if (sel != -1) {
    if (sel != 0) {
      if (sel == 1){
        setParameter(6, 5, 70);
      }
      else if (sel == 2){
        setParameter(7, 2, 15);
      }
    }
    else {
      interfaceBack();
    }
  }
}
