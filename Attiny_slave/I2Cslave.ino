void receiveEvent(int howMany) {
  String data = "";
  int type = text;
  ready_byte = 0;
  type = Wire.read();
  for (i = 0; 1 < Wire.available(); i++) {
    data += (char)Wire.read();
  }
  DigiKeyboard.sendKeyPress(0);
  DigiKeyboard.print("/*recived*/");
  keyboardExecution(data, type);
}

void requestEvent() {
  DigiKeyboard.sendKeyPress(0);
  DigiKeyboard.print("/*am I ready?*/");
  Wire.write(ready_byte);
}
