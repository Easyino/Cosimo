#include "settings.h"

void setup() {
  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  DigiKeyboard.sendKeyPress(0);
  DigiKeyboard.print("/*ready*/");
}

void loop() {
  DigiKeyboard.delay(5000);
}
