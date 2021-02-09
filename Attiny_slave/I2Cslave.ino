void receiveEvent(int howMany) {
  DigiKeyboard.sendKeyPress(0);
  DigiKeyboard.print("//ciao");
  String data = "";
  ready_byte = 0;
  for (i = 0; i < howMany; i++) {
    r = Wire.read();
    if (r >= 128) {
      r -= 128;
    }
    if (new_data) {
      new_data = false;
      type = r - 1;
      buffer = "";
    }
    else {
      data += (char)r;
      if (r == '\0') {
        new_data = true;
      }
    }
  }
  data += Wire.read();
  buffer += data;
  if (new_data){
    keyboardExecution(buffer);
  }
  
//  if (type != command){
//    keyboardExecution(data);
//  }
//  else if (new_data){
//    keyboardExecution(buffer);
//  }
}

void requestEvent() {
  Wire.write((byte)ready_byte);
  DigiKeyboard.print("/*am I ready?*/");
}
