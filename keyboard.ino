String commandTable(int command) {
  switch (command) {
    case 0: return "POWER DOWN";
    case 1: return "SLEEP";
    case 2: return "WAKE UP";
    case 3: return "PLAY";
    case 4: return "PAUSE";
    case 5: return "RECORD";
    case 6: return "FAST FORWARD";
    case 7: return "REWIND";
    case 8: return "FAST FORWARD";
    case 9: return "NEXT TRACK";
    case 10: return "STOP";
    case 11: return "MUTE";
    case 12: return "VOLUME INC";
    case 13: return "VOLUME DEC";
    case 14: return "RANDOM PLAY";
    case 15: return "ESC";
    case 16: return "BACKSPACE";
    case 17: return "TAB";
    case 18: return "CAPS LOCK";
    case 19: return "F1";
    case 20: return "F2";
    case 21: return "F3";
    case 22: return "F4";
    case 23: return "F5";
    case 24: return "F6";
    case 25: return "F7";
    case 26: return "F8";
    case 27: return "F9";
    case 28: return "F10";
    case 29: return "F11";
    case 30: return "F12";
    case 31: return "CTRL";
    case 32: return "SHIFT";
    case 33: return "ALT";
    case 34: return "GUI";
    case 35: return "MENU";
    case 36: return "PRINTSCREEN";
    case 37: return "SCROLL LOCK";
    case 38: return "PRINTSCREEN";
    case 39: return "SCROLL LOCK";
    case 40: return "HOME";
    case 41: return "ENTER";
    case 42: return "PAGE UP";
    case 43: return "DELETE";
    case 44: return "END";
    case 45: return "PAGE DOWN";
    case 46: return "RIGHT";
    case 47: return "LEFT";
    case 48: return "DOWN";
    case 49: return "UP";
    case 50: return "NUM LOCK";
    case 51: return "A";
    case 52: return "B";
    case 53: return "C";
    case 54: return "D";
    case 55: return "E";
    case 56: return "F";
    case 57: return "G";
    case 58: return "H";
    case 59: return "I";
    case 60: return "J";
    case 61: return "K";
    case 62: return "L";
    case 63: return "M";
    case 64: return "N";
    case 65: return "O";
    case 66: return "P";
    case 67: return "Q";
    case 68: return "R";
    case 69: return "S";
    case 70: return "T";
    case 71: return "U";
    case 72: return "V";
    case 73: return "W";
    case 74: return "X";
    case 75: return "Y";
    case 76: return "Z";
  }
}


String stringToCommand(String data) {
  String  command_global;
  String command_single;
  String confrontable_command_table = commandTable(0);
  confrontable_command_table.trim();
  int number = 0;
  int command;
  data.trim();
  for (a = 0; data[q] != '\0'; a += q) {
    command_single = "";
    for (q = 0; data[q] != '*' && data[q] != '+' && data[q] != '\0'; q++) {
      if (number) {
        number = data[q];
        if (number > 100) {
          number = 100;
        }
        command_single += number;
      }
      else {
        if (data[q] != '_') {
          command_single += data[q];
        }
      }
    }
    command_single.toUpperCase();
    for (command = 1; command_single != confrontable_command_table; command++) {
      confrontable_command_table = commandTable(command);
      confrontable_command_table.trim();
      if (command > 76) {
        return "error";
      }
    }
    command_global += command + 128 - 1;
    if (data[q] == '+') {
      command_global += 253;
    }
    if (data[q] == '*') {
      command_global += 254;
      number = 1;
    }
  }
  return command_global;
}

String commandToString(String command) {
  String command_global;
  for (q = 0; command[q] != '\0'; q++) {
    if (command[q] == 253) {
      command_global += " + ";
    }
    else if (command[q] == 253) {
      command_global += " * ";
    }
    else {
      if (command[q - 1] == 253) {
        command_global.concat(command[q]);
      }
      else {
        command_global += commandTable(command[q]);
      }
    }
  }
}
