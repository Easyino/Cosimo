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
int settings[50];

int checkpoint_memory[20];
int checkpoint_jump;

int command_length[50];
//short int changes[50];
int sector_loaded = 0;

int i, a, r, c, q, d, e, f;


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
typedef struct {
  byte aligned = left;
  byte x;
  byte y;
  String data;
} elements;
elements element[10];
int element_counter = 0;
int interface = 0;
int loaded_interface = -1;


//Encryption
namespace TypeCast = experimental::TypeConversion;
using namespace experimental::crypto;
uint8_t resultArray[SHA256::NATURAL_LENGTH] { 0 };
uint8_t derivedKey[ENCRYPTION_KEY_LENGTH] { 0 };
char masterKey[] = "0       ";
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
#define bouncing_time 2
unsigned long int last_millis;
unsigned long int millis_gap;
enum buttons{
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
int ret=20; //number of retrys for connceting to your local network. if you have got a slow connection we advice you to increase the number e.g=50
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
