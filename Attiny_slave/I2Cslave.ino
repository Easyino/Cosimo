void receiveEvent(int howMany) {
  int r, i;
  bool missing = true;
  //digitalWrite(READY, 1);
  while (1 < Wire.available()) {
    last:
    r = Wire.read();
    if (r == 125) {
      recived++;
      new_data = 1;
    }
    else if (r == 126) {
      execute = 1;
    }
    else if (new_data){
      new_data = 0;
      memory_type[recived] = r;
    }
    else {
      memory_map[recived] += char(r);
    }
    if (!missing){
      //digitalWrite(READY, 0);
      return;
    }
  }
  missing = false;
  goto last;
}
