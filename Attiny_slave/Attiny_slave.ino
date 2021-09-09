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
  if (execute){
    execute = 0;
    keyboardExecution();
    for (int i = 0; i <= recived; i++) {
      memory_type[i] = 0;
      memory_map[i] = "";
    }
    recived = 0;
  }
  if (millis() - last_millis > 1000){
    last_millis = millis();
    led_state = !led_state;
    digitalWrite(READY, led_state);
  }
}