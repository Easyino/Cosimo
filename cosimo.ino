 #include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "Easyino Cosimo";
const char* password = "12345678"; 
IPAddress local_ip(10, 10, 10, 1);
IPAddress gateway(10, 10, 10, 1);
IPAddress subnet(255, 0, 0, 0);
ESP8266WebServer server(80);

struct partizione{
  String nome;
  String email;
  String password;
};
int n_partizioni=0;
struct partizione partizioni[10];

void setup() {

  Serial.begin(115200);

//GESTIONE SERVER
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  server.on("/", handle_form);
  server.on("/conf", handle_conf);
  server.begin();
}

void loop() {
  server.handleClient();

}
