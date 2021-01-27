#include "DigiKeyboard.h"
#include <Wire.h>
String buffer;
int type;
bool new_data = true;
int i, a, c, r;
byte ready_byte = 1;
enum types {
  text,
  command,
  password
};
