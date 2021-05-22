void WiFiOn() {
  wifi_fpm_do_wakeup();
  wifi_fpm_close();
  wifi_set_opmode(STATION_MODE);
  wifi_station_connect();
}

void WiFiOff() {
  wifi_station_disconnect();
  wifi_set_opmode(NULL_MODE);
  wifi_set_sleep_type(MODEM_SLEEP_T);
  wifi_fpm_open();
  wifi_fpm_do_sleep(0xFFFFFFF);
}


void createNetwork() {
  Serial.println("creo la rete...");
  WiFi.softAP(personal_ssid, personal_password);
  //WiFi.softAPConfig(local_ip, gateway, subnet);
  server.on("/", handle_getNetInfo);
  server.on("/conf", handle_confNetInfo);
  server.begin();
  netStat = 1;
  Serial.println("Fatto");
  //interface = wifiCreateInter;
}

void tryConnect() {
  loadNetData();
  Serial.print("trying connecting to: ");
  Serial.print(ext_ssid);
  Serial.print("  with this password:");
  Serial.println(ext_password);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ext_ssid, ext_password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {
    display.clear();
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_RIGHT);
    display.drawString(128, 52, "Connecting...");
    display.display();
    delay(500);
    Serial.print(".");
    ret--;
    if (ret <= 0) {

      display.clear();
      display.setTextAlignment(TEXT_ALIGN_RIGHT);
      display.drawString(128, 52, "Failed!");
      display.display();

      Serial.println("FAILED!");
      EEPROM.write(1, 0); //set netmode to "create a wifi"
      EEPROM.commit();
      ESP.restart();
    }
  }
  Serial.println("");
  Serial.print("IP address:  ");
  Serial.println(WiFi.localIP());
  wifi_IP = WiFi.localIP().toString();
  server.on("/", handle_conncetionSuccess);
  server.on("/all", handle_GetAllJson);
  server.on("/get", handle_GetSingleJson);
  server.on("/set", handle_Set);
  server.on("/delete", handle_delete);
  server.begin();
  Serial.println("I'm connected!");
  netStat = 1;
  Serial.println("SUCCESS!!!");
  EEPROM.write(1, 1); //set netmode to "try connect"
  EEPROM.commit();
}