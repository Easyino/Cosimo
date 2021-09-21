
void keyboardExecution() {
  int i, c, a;
  bool combination = 0;
  for (a = 0; a < recived; a++) {
    if (true) {
      DigiKeyboard.print(memory_map[a]);
    }
    else if (memory_type[a] == command){
      bool number = false;
      for (i = 0, c = 0; memory_map[i] != '\0'; i++) {
        if (number == true) {
          c += memory_map[a][i];
          memory_map[a][i] = 0;
        }
        else if (memory_map[a][i] == 123) {
          //memory_map[a][i] = 0;
          combination = 1;
        }
        if (memory_map[a][i] == 124) {
          number = true;
          memory_map[a][i] = 0;
        }
      }
      for (; c > 0; c--) {
        for (i = 0; memory_map[a][i] != '\0'; i++) {
          DigiKeyboard.sendKeyPress(commandTable[(int)(memory_map[a][0])], (combination ? 0 : commandTable[(int)(memory_map[a][2])]));
        }
        DigiKeyboard.delay(20);
        DigiKeyboard.sendKeyPress(0);
      }
    }
  }
}