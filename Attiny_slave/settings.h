#define LAYOUT_ITALIAN
#include "DigiKeyboard.h"
#include <Wire.h>
String buffer;
String memory_map[5];
byte memory_type[5];
bool new_data = true;
int recived = -1;
enum types {
  zero,
  text,
  command,
  password,
  comunication
};
