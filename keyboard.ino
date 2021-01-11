String reverseCommandTable[76]
{"POWERDOWN", "WAKEUP", "SLEEP", "PLAY", "PAUSE", "RECORD", "FASTFORWARD", "REWIND", "NEXTTRACK", "PREVTRAK", "STOP", "EJECT", "MUTE", "VOLUMEINC", 
"VOLUMEDEC", "ENTER", "ESC", "BACKSPACE", "TAB", "SPACE", "CAPSLOCK", "F1", "F2", "F3", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12",
"PRINTSCREEN", "SCROLLLOCK", "HOME", "PAGEUP", "PAGEDOWN", "DELETE", "END", "RIGHT", "LEFT", "DOWN", "UP", "NUMLOCK", "A", "B", "C", "D", "E", "F", "G",
"H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "CTRL", "SHIFT", "ALT", "GUI"};
String commandTable(int com) {
  switch (com) {
    case 0: return "POWER DOWN";
    case 1: return "WAKE UP";
    case 2: return "SLEEP";
    case 3: return "PLAY";
    case 4: return "PAUSE";
    case 5: return "RECORD";
    case 6: return "FAST FORWARD";
    case 7: return "REWIND";
    case 8: return "NEXT TRACK";
    case 9: return "PREV TRACK";
    case 10: return "STOP";
    case 11: return "EJECT";
    case 12: return "MUTE";
    case 13: return "VOLUME INC";
    case 14: return "VOLUME DEC";
    case 15: return "ENTER";
    case 16: return "ESC";
    case 17: return "BACKSPACE";
    case 18: return "TAB";
    case 19: return "SPACE";
    case 20: return "CAPS LOCK";
    case 21: return "F1";
    case 22: return "F2";
    case 23: return "F3";
    case 24: return "F4";
    case 25: return "F5";
    case 26: return "F6";
    case 27: return "F7";
    case 28: return "F8";
    case 29: return "F9";
    case 30: return "F10";
    case 31: return "F11";
    case 32: return "F12";
    case 33: return "PRINTSCREEN";
    case 34: return "SCROLL LOCK";
    case 35: return "HOME";
    case 36: return "PAGE UP";
    case 37: return "PAGE DOWN";
    case 38: return "DELETE";
    case 39: return "END";
    case 40: return "RIGHT";
    case 41: return "LEFT";
    case 42: return "DOWN";
    case 43: return "UP";
    case 44: return "NUM LOCK";
    case 45: return "A";
    case 46: return "B";
    case 47: return "C";
    case 48: return "D";
    case 49: return "E";
    case 50: return "F";
    case 51: return "G";
    case 52: return "H";
    case 53: return "I";
    case 54: return "J";
    case 55: return "K";
    case 56: return "L";
    case 57: return "M";
    case 58: return "N";
    case 59: return "O";
    case 60: return "P";
    case 61: return "Q";
    case 62: return "R";
    case 63: return "S";
    case 64: return "T";
    case 65: return "U";
    case 66: return "V";
    case 67: return "W";
    case 68: return "X";
    case 69: return "Y";
    case 70: return "Z";
    case 71: return "CTRL";
    case 72: return "SHIFT";
    case 73: return "ALT";
    case 74: return "GUI";
    default: return "ERROR 404";
  }
}


String stringToCommand(String data) {
  String command_global;
  String command_single;
  int number = 0;
  int com;
  data.trim();
  for (a = 0, q = 0; data[a] != '\0'; a += q) {
    command_single = "";
    for (q = 0; data[a + q] != '*' && data[a + q] != '+' && data[a + q] != '\0'; q++) {
      if (number) {
        number = data[a + q];
        if (number > 100) {
          number = 100;
        }
        command_single += (char)number;
      }
      else {
        if (data[a + q] != '_') {
          command_single += (char)data[a + q];
        }
      }
    }
    command_single.toUpperCase();
    for (com = 0; command_single != reverseCommandTable[com]; com++) {
      if (com > 74) {
        return "error";
      }
    }
    command_global += (char)(com + 128 - 1);
    if (data[a + q] == '+') {
      command_global += 253;
      q++;
    }
    else if (data[a + q] == '*') {
      command_global += 254;
      number = 1;
      q++;
    }
  }
  return command_global;
}

String commandToString(String com) {
  String command_global;
  for (q = 0; com[q] != '\0'; q++) {
    if (com[q] == 253) {
      command_global += " + ";
    }
    else if (com[q] == 253) {
      command_global += " * ";
    }
    else {
      if (com[q - 1] == 253) {
        command_global.concat(com[q]);
      }
      else {
        command_global += commandTable(com[q]);
      }
    }
  }
}
