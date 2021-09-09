unsigned int commandTable[76] {
  KEY_SYSTEM_POWER_DOWN, KEY_SYSTEM_WAKE_UP, KEY_SYSTEM_SLEEP, KEY_MEDIA_PLAY, KEY_MEDIA_PAUSE, KEY_MEDIA_RECORD, KEY_MEDIA_FAST_FORWARD, KEY_MEDIA_REWIND, KEY_MEDIA_NEXT_TRACK, KEY_MEDIA_PREV_TRACK, KEY_MEDIA_STOP, KEY_MEDIA_EJECT, KEY_MEDIA_MUTE, KEY_MEDIA_VOLUME_INC,
  KEY_MEDIA_VOLUME_DEC, KEY_ENTER, KEY_ESC, KEY_BACKSPACE, KEY_TAB, KEY_SPACE, KEY_CAPS_LOCK, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12,
  KEY_PRINTSCREEN, KEY_SCROLL_LOCK, KEY_HOME, KEY_PAGE_UP, KEY_PAGE_DOWN, KEY_DELETE, KEY_END, KEY_RIGHT, KEY_LEFT, KEY_DOWN, KEY_UP, KEY_NUM_LOCK, KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G,
  KEY_H, KEY_I, KEY_J, KEY_K, KEY_L, KEY_M, KEY_N, KEY_O, KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T, KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z, KEY_LEFT_CTRL, KEY_LEFT_SHIFT, KEY_LEFT_ALT, KEY_LEFT_GUI
};
void keyboardExecution() {
  int i, c, a;
  for (a = 0; a <= recived; a++) {
    if (true) { /*memory_type[a] == text || memory_type[a] == password*/1110
      DigiKeyboard.sendKeyPress(0);
      DigiKeyboard.print(String(memory_type[a]));
    }
    else if (memory_type[a] == command) {
      bool number = false;
      for (i = 0, c = 0; memory_map[i] != '\0'; i++) {
        if (number == true) {
          c += memory_map[a][i];
          memory_map[a][i] = 0;
        }
        else if (memory_map[a][i] == 123) {
          memory_map[a][i] = 0;
        }
        if (memory_map[a][i] == 124) {
          number = true;
          memory_map[a][i] = 0;
        }
      }
      for (; c > 0; c--) {
        for (i = 0; memory_map[a][i] != '\0'; i++) {
          DigiKeyboard.sendKeyPress(commandTable[(int)(memory_map[a][i])]);
        }
        DigiKeyboard.delay(20);
        DigiKeyboard.sendKeyPress(0);
      }
    }
  }
}
