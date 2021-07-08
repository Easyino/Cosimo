#include "settings.h"
void setup() {
  pinMode(1, OUTPUT);
  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  DigiKeyboard.enableLEDFeedback();
  DigiKeyboard.sendKeyPress(0);
}

void loop() {
  DigiKeyboard.delay(1);
}
