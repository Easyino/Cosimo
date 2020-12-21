

void handle_conf() {
  Serial.println("Qualcuno mi ha fatto una richiesta.../n");
  section[n_section].title = server.arg("Title");
  section[n_section].email = server.arg("Email");
  section[n_section].password = server.arg("Password");
  Serial.print("nome:");
  Serial.print(section[n_section].title);
  Serial.print(" Email:");
  Serial.print(section[n_section].email);
  Serial.print("Password:");
  Serial.print(section[n_section].password);

  server.send(200, "text/html", "<meta http-equiv='refresh' content='1; URL=/' >");
}

void handle_form() {

  server.send(200, "text/html", pgForm());
}


void handle_getNetInfo() {

  server.send(200, "text/html", pgGetNetInfo());
}

void handle_confNetInfo() {
  Serial.println("Qualcuno mi ha fatto una richiesta.../n");
  newWriteString(addrExtSSID,server.arg("SSID"));
   newWriteString(addrExtSSID, server.arg("Password"));

  Serial.print("SSID:");
  Serial.print(newReadString(addrExtSSID));

  Serial.print("Password:");
  Serial.print(newReadString(addrExtPassword));

  server.send(200, "text/html", "<meta http-equiv='refresh' content='1; URL=/' >");
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
  Serial.println("cerco di connettermi");
  int ret = 0;
  WiFi.mode(WIFI_STA);
  WiFi.begin(ext_ssid, ext_password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {

    delay(500);
    Serial.print(".");
    ret++;
    if (ret > 10){
      Serial.println("fallito!");
      return false;
    }
  }


  Serial.println("");

  Serial.print("indirizzoIP:  ");
  Serial.println(WiFi.localIP());


  server.on("/", handle_form);
  server.on("/conf", handle_conf);

  server.begin();
  Serial.println("mi sono connesso!");
  netStat = 1;
    Serial.println("SUCCESS!!!");
  return true;
}
