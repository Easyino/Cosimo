#include "settings.h"

void setup() {
  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
}

void loop() {
  DigiKeyboard.delay(5000);
}
