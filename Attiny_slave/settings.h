#define LAYOUT_ITALIAN
#include "DigiKeyboard.h"
#include <Wire.h>
#define READY 1
String buffer;
String memory_map[5];
bool memory_type[5];
bool new_data = 1;
bool execute;
byte recived;
enum types {
  null,
  text,
  command,
  password,
  comunication
};
bool led_state;