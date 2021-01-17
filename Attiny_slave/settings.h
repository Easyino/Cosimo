#include "DigiKeyboard.h"
#include <Wire.h>

int i, a, c, r;
int ready_byte = 1;
enum types {
  text,
  command,
  password
};
