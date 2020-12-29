void handle_getNetInfo() {

  server.send(200, "text/html", pgGetNetInfo());
}

void handle_confNetInfo() {
  Serial.println("Qualcuno mi ha fatto una richiesta.../n");

  loadSector(1);

//  updateString(addrExtSSID, server.arg("SSID"));
//  updateString(addrExtPassword, server.arg("Password"));
//  
//  updateEEPROM();
  Serial.print("SSID:");
  Serial.print(memory_map[addrExtSSID]);

  Serial.print("Password:");
  Serial.print(memory_map[addrExtPassword]);

  server.send(200, "text/html", "<meta http-equiv='refresh' content='1; URL=/' >");
  ESP.restart();
}



void createNetwork() {
  Serial.println("creo la rete...");
  WiFi.softAP(personal_ssid, personal_password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  server.on("/", handle_getNetInfo);
  server.on("/conf", handle_confNetInfo);
  server.begin();
  netStat = 1;
  Serial.print("Fatto");

}

bool tryConnect() {
  loadNetData();
  Serial.print("trying connecting to: ");
  Serial.print(ext_ssid);
  Serial.print("  with this password:");
  Serial.println(ext_password);
  int ret = 0;
  WiFi.mode(WIFI_STA);
  WiFi.begin(ext_ssid, ext_password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {

    delay(500);
    Serial.print(".");
    ret++;
    if (ret > 10) {
      Serial.println("fallito!");
      EEPROM.write(0, 0); //set netmode to "create a wifi"
      EEPROM.commit();
      ESP.restart();
      return false;
    }
  }


  Serial.println("");

  Serial.print("indirizzoIP:  ");
  Serial.println(WiFi.localIP());


  //server.on("/", handle_form);
  //server.on("/conf", handle_conf);

  server.begin();
  Serial.println("mi sono connesso!");
  netStat = 1;
  Serial.println("SUCCESS!!!");
  return true;
}
