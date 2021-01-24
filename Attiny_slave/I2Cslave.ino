void receiveEvent(int howMany) {
  String data = "";
  int type = text;
  ready_byte = 0;
  for (i = 0; 1 < Wire.available(); i++) {
    data += (char)Wire.read();
  }
  type = Wire.read() - 1;
  DigiKeyboard.sendKeyPress(0);
  DigiKeyboard.print("/**/");
  keyboardExecution(data, type);
}

void requestEvent() {
  Wire.write((byte)ready_byte);
  DigiKeyboard.sendKeyPress(0);
  DigiKeyboard.print("/*am I ready?*/");
}
