void receiveEvent(int howMany) {
  int r, i;
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
        recived = -1;
//        for (i = 0; memory_map[i] != ""; i++) {
//          memory_type[i] = 0;
//          memory_map[i] = "";
//        }
      }
      else {
        memory_map[recived] += char(r);
      }
    }
  }
  goto last;
}
