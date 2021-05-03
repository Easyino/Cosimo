#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

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
#define EEPROM_offset 2
/*number of spaces in eeprom reserved:
   Bit 0: state of network(1=connect | 0=create)
   Bit 1: number of attempts already done of the pin
*/

#define usable_address_bits 7
int max_value_address = 0;

String memory_map[50];
int memory_type[50] = { -1};
enum types {
  text,
  command,
  password
};
String data_types[3] {"text", "command", "password"};
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
  return -1;
}

int checkpoint_memory[20];
int checkpoint_jump;
int checkpoint_max = 0;

int length;
//short int changes[50];
int sector_loaded = 0;

unsigned int i, a, r, c, q, d, e, f;


//Commands
enum credential_commands {
  addrExtSSID,
  addrExtPassword
};

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
enum alignment {
  left,
  right,
  center
};
enum interfaces {
  pinInter,
  firstConfigInter,
  timeInter,
  logInter,
  questionInter,
  menuInter,
  settingsInter,
  commandInter,
  wifiCreateInter
};
typedef struct {
  bool title = false;
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

int element_counter = 0;
int special_element_counter = 0;
int interface = menuInter;
int loaded_interface = commandInter;
int previous_interface;
bool oled_updated = false;
String elements_list[20] = {"testo-1", "testo-2", "testo-3", "testo-4", "testo-5", "testo-6", "testo-7", "testo-8"};
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
#define scrolling_time 100
#define bouncing_time 0
unsigned long int last_millis;
unsigned long int millis_gap;
enum buttons {
  up,
  confirm,
  down
};


//Variables declaration
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
const char* personal_ssid = "Easyino Cosimo";//you can change your cosimo's SSID here
const char* personal_password = "12345678"; //you can change your cosimo's password here
String ext_ssid;
String ext_password;
String wifi_IP = "Not connected";
IPAddress local_ip(10, 10, 10, 1);
IPAddress gateway(10, 10, 10, 1);
IPAddress subnet(255, 0, 0, 0);
ESP8266WebServer server(80);

#define addrNetMode 0 //0 for creating a webpage; 1 for connecting to an existing one
