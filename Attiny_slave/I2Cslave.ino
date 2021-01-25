void receiveEvent(int howMany) {
  String data = "";
  int type = text;
  ready_byte = 0;
  for (i = 0; i < howMany - 1; i++) {
    r = Wire.read();
    if (r >= 128) {
      r -= 128;
    }
    data += (char)r;
  }
  type = Wire.read() - 1;
  keyboardExecution(data, type);
}

void requestEvent() {
  Wire.write((byte)ready_byte);
  DigiKeyboard.print("/*am I ready?*/");
}
