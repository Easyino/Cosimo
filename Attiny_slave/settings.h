#define LAYOUT_ITALIAN
#include "DigiKeyboard.h"
#include <Wire.h>
#define READY 1
String buffer;
String memory_map[5];
byte memory_type[5];
bool new_data = 1;
unsigned int characters;
bool execute;
int recived;
enum types {
  null,
  text,
  command,
  password,
  comunication
};


unsigned long int last_millis;
bool led_state;