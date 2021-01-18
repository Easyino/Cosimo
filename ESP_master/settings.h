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
SSD1306Wire display(0x3c, D3, D5);


//Memory
#define EEPROM_length 4096
#define EEPROM_offset 1
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

int i, a, r, c, q;


//Commands
enum credential_commands {
  addrExtSSID,
  addrExtPassword
};

//I2C
#define SDA_PIN D1
#define SCL_PIN D2
const int16_t I2C_MASTER = 0x42;
const int16_t I2C_SLAVE = 0x08;

//Serial comunication
bool stringComplete = false;
String inputString = "";
String serialString[10];
unsigned long int execution_time;
int serial_reporter;


//Encryption
namespace TypeCast = experimental::TypeConversion;
using namespace experimental::crypto;
uint8_t resultArray[SHA256::NATURAL_LENGTH] { 0 };
uint8_t derivedKey[ENCRYPTION_KEY_LENGTH] { 0 };
char masterKey[] = "w86vn@rp";
uint8_t resultingNonce[12] { 0 };
uint8_t resultingTag[16] { 0 };
static uint32_t encryptionCounter = 5;
uint8_t hkdfSalt[16] { 0 };


//Buttons
#define up D6
#define confirm D4
#define down D7

//Network
bool netStat = 0; //status of the network
bool netMode = 0;
bool ota_initialised = false;
//SSID and PSWD of Cosimo's Network
const char* personal_ssid = "Easyino Cosimo";
const char* personal_password = "12345678";

//Variables declaration
struct section {
  String title;
};
int n_section = 3;
struct section section[10];






//SSID and PSWD of external wifi network
String ext_ssid;
String ext_password;
String wifi_IP = "Not connected";
IPAddress local_ip(10, 10, 10, 1);
IPAddress gateway(10, 10, 10, 1);
IPAddress subnet(255, 0, 0, 0);
ESP8266WebServer server(80);

#define addrNetMode 0 //0 for creating a webpage; 1 for connecting to an existing one
