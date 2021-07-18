#include "settings.h"
void setup() {
  pinMode(READY, OUTPUT);
  digitalWrite(READY, 0);
  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  //DigiKeyboard.enableLEDFeedback();
  DigiKeyboard.sendKeyPress(0);
}

void loop() {
  DigiKeyboard.delay(1);
}
