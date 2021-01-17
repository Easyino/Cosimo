void receiveEvent(int howMany) {
  String data = "";
  int type;
  ready_byte = 0;
  type = Wire.read();
  for (i = 0; 1 < Wire.available(); i++) {
    data += (char)Wire.read();
  }
  keyboardExecution(data, type);
}

void requestEvent() {
  Wire.write(ready_byte);
}
