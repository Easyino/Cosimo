void receiveEvent(int howMany) {
  int r, i;
  bool missing = true;
  while (1 < Wire.available()) {
    last:
    r = Wire.read();
    if (new_data) {
      new_data = false;
      recived++;
      memory_type[recived] = r;
    }
    else {
      if (r == 125) {
        new_data = true;
      }
      else if (r == 126) {
        keyboardExecution();
        for (i = 0; i <= recived; i++) {
          memory_type[i] = 0;
          memory_map[i] = "";
        }
        recived = -1;
      }
      else {
        memory_map[recived] += char(r);
      }
    }
    if (!missing){
      return;
    }
  }
  missing = false;
  goto last;
}
