void receiveEvent(int howMany) {
  int r, i;
  bool missing = true;
  //digitalWrite(READY, 1);
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
        execute = 1;
      }
      else {
        memory_map[recived] += char(r);
      }
    }
    if (!missing){
      //digitalWrite(READY, 0);
      return;
    }
  }
  missing = false;
  goto last;
}
