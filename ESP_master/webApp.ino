void handle_getNetInfo() {
  server.send(200, "text/html", pgGetNetInfo());
}

void handle_confNetInfo() {
  Serial.println("Qualcuno mi ha fatto una richiesta.../n");
  loadSector(1);
  updateCommand(addrExtSSID, server.arg("SSID"), text);
  updateCommand(addrExtPassword, server.arg("Password"), password);
  updateEEPROM();
  Serial.print("SSID:");
  Serial.print(memory_map[addrExtSSID]);
  Serial.print("Password:");
  Serial.print(memory_map[addrExtPassword]);
  //  server.send(200, "text/html", "ORA MI RIAVVIO<meta http-equiv='refresh' content='1; URL=/' >");
  server.send(200, "text/html", "ORA MI RIAVVIO");
  EEPROM.write(0, 1); //set netmode to "try connect"
  demoSectors();//To remove
  EEPROM.commit();
  ESP.restart();
}

void handle_conncetionSuccess() {
  server.send(200, "text/html", "ti sei connesso con successo alla rete");
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
  interface=wifiCreateInter;
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
      EEPROM.write(0, 0); //set netmode to "create a wifi"
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
}
