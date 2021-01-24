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
    Serial.println("\nUpdate display element");
    element[number].data = new_data;
    loadDisplay();
  }
}

void loadDisplay() {
  display.clear();
  display.flipScreenVertically();
  display.setContrast(255);
  display.setFont(ArialMT_Plain_10);
  for (int i = 0; i < element_counter; i++) {
    if (element[i].aligned == left) {
      display.setTextAlignment(TEXT_ALIGN_LEFT);
      Serial.print("left ");
    }
    else{
      display.setTextAlignment(TEXT_ALIGN_RIGHT);
      Serial.print("right ");
    }
    display.drawString(element[i].x, element[i].y, element[i].data);
    Serial.print(element[i].x);
    Serial.print(" ");
    Serial.print(element[i].y);
    Serial.print(" ");
    Serial.println(element[i].data);
  }
  display.display();
}
