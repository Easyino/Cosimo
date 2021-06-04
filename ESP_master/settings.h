#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "user_interface.h"

#include <ArduinoOTA.h>

#include <EEPROM.h>
#include <Wire.h>
#include <PolledTimeout.h>
#include <TypeConversion.h>
#include <Crypto.h>

#include "SSD1306Wire.h"
#include "images.h"
SSD1306Wire display(0x3c, D2, D1);//D3, D5


//Memory
#define EEPROM_length 4096
#define EEPROM_offset 5
/*number of spaces in eeprom reserved:
   Bit 0: state of network(1=connect | 0=create)
   Bit 1: number of attempts already done of the pin
*/

#define usable_address_bits 7
int max_value_address = 0;

String memory_map[50];
int memory_type[50];
enum types {
  null,
  text,
  command,
  password
};

String data_types[4] {"null", "text", "command", "password"};
int dataTypes(String type) {
  if (type.equalsIgnoreCase("text")) {
    return text;
  }
  else if (type.equalsIgnoreCase("command")) {
    return command;
  }
  else if (type.equalsIgnoreCase("password")) {
    return password;
  }
  return null;
}

int checkpoint_memory[20];
int checkpoint_jump;


int length;
int sector_loaded = 0;
int sector_max = 0;

int i, a, r, c, q, d, e, f;


//I2C
const int16_t I2C_MASTER = 0x42;
const int16_t I2C_SLAVE = 0x08;

//Serial comunication
bool stringComplete = false;
String inputString = "";
String serialString[10];
unsigned long int execution_time;
int serial_reporter;

//Oled
bool orientation;
byte n_rows;
enum alignment {
  left,
  right,
  center
};
enum interfaces {
  back = -1,
  pinInter,
  firstConfigInter,
  wifiCreateInter,
  timeInter,
  logInter,
  displayInter,
  menuInter,
  commandInter,
  wifiInter,
  settingsInter,
  questionInter
};

typedef struct {
  byte font;
  byte limit;
  byte aligned;
  byte x;
  byte y;
  String data;
} elements;
elements element[10];

typedef struct {
  byte type;
  byte x;
  byte y;
  byte height;
  byte width;
} special_elements;
special_elements special_element[10];

enum special_element_types {
  rect,
  circle,
  filledCircle,
  rectangle,
  filledRectangle
};

typedef struct {
  byte type;
  byte x;
  byte y;
  byte ref;
} icon_elements;
icon_elements icon_element[10];

struct parameters{
  int address;
  int data = -1;
  int min;
  int max;
};
parameters parameter;

int element_counter = 0;
int special_element_counter = 0;
int icon_element_counter = 0;
int interface = menuInter;
int loaded_interface = timeInter;
int previous_interface;
bool oled_updated = false;
String elements_list[20];
bool title_list;
int begin_list;
int dialog_interface;

//Encryption
namespace TypeCast = experimental::TypeConversion;
using namespace experimental::crypto;
uint8_t resultArray[SHA256::NATURAL_LENGTH] { 0 };
uint8_t derivedKey[ENCRYPTION_KEY_LENGTH] { 0 };
char masterKey[] = "        ";
uint8_t resultingNonce[12] { 0 };
uint8_t resultingTag[16] { 0 };
static uint32_t encryptionCounter = 5;
uint8_t hkdfSalt[16] { 0 };
bool wrong_key = false;
#define chances 5

//Buttons
#define button_up D5
#define button_confirm D6
#define button_down D7
#define scrolling_time 100 // The unit number has to be = to 0
#define bouncing_time 20
unsigned long int last_millis;
unsigned long int millis_gap;
enum buttons {
  up,
  confirm,
  down
};
int buttonPressed;
int previousButton;
int triggButton = -1;


//Variables declaration    /////////////////////////////// To remove (only for Longhino)
struct section {
  String title;
};
int n_section = 3;
struct section section[10];


//SSID and PSWD of external wifi network

bool netStat = 0; //status of the network
bool netMode = 0;
bool ota_initialised = false;
/*da togliere!!!!*/int ret = 20; //number of retrys for connceting to your local network. if you have got a slow connection we advice you to increase the number e.g=50
//SSID and PSWD of Cosimo's Network
ESP8266WebServer server(80);
const char* personal_ssid = "Easyino Cosimo";//you can change your cosimo's SSID here
const char* personal_password = "12345678"; //you can change your cosimo's password here
String ext_ssid;
String ext_password;
String wifi_IP = "Not connected";
/*
IPAddress local_ip(10, 10, 10, 1);
IPAddress gateway(10, 10, 10, 1);
IPAddress subnet(255, 255, 255, 0);
*/

#define addrNetMode 0 //0 for creating a webpage; 1 for connecting to an existing one