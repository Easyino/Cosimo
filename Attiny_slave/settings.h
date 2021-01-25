#include "DigiKeyboard.h"
#include <Wire.h>
int i, a, c, r;
byte ready_byte = 1;
enum types {
  text,
  command,
  password
};
