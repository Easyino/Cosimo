//Memory
#define EEPROM_lenght 4096
#define usable_address_bits 7
#define settings_bytes 50
int max_value_address = 0;

//byte memory_map[500];
String memory_map[50];
int settings[settings_bytes];

int checkpoint_memory[20];
//unsigned int address_bytes;

int command_length[50];
//short int changes[50];
int sector_loaded = 0;

int i, a, r, c;



//Variables declaration
struct section{
  String title;
  String email;
  String password;
};
int n_section=0;
struct section section[10];


//Serial comunication
bool stringComplete = false;
String inputString = "";



//Network
bool netStat=0;//status of the network
bool netMode=0;
//SSID and PSWD of Cosimo's Network
const char* personal_ssid = "Easyino Cosimo";
const char* personal_password = "12345678"; 

//SSID and PSWD of external wifi network  
String ext_ssid = "federixo-Inspiron-5567";
String ext_password = "pasword"; 
IPAddress local_ip(10, 10, 10, 1);
IPAddress gateway(10, 10, 10, 1);
IPAddress subnet(255, 0, 0, 0);
ESP8266WebServer server(80);



//EEPROM

#define addrNetMode 0 //0 for creating a webpage; 1 for connecting to an existing one
enum credential_commands {
  addrExtSSID,
  addrExtPassword
};
