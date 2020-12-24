


//variables declaration
struct section{
  String title;
  String email;
  String password;
};
int n_section=0;
struct section section[10];




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
#define addrExtSSID 1
#define addrExtPassword 2