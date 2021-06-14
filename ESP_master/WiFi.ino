void WiFiOn() {
  wifi_fpm_do_wakeup();
  wifi_fpm_close();
  wifi_set_opmode(STATION_MODE);
  wifi_station_connect();
  wifi_IP = WiFi.localIP().toString();
}

void WiFiOff() {
  wifi_station_disconnect();
  wifi_set_opmode(NULL_MODE);
  wifi_set_sleep_type(MODEM_SLEEP_T);
  wifi_fpm_open();
  wifi_fpm_do_sleep(0xFFFFFFF);
  wifi_IP = WiFi.localIP().toString();
}


void createNetwork() {
  Serial.println("creo la rete...");
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, netMsk);
  WiFi.softAP(personal_ssid, personal_password);
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(DNS_PORT, "*", apIP);
  server.on("/", handle_getNetInfo);
  server.on("/conf", handle_confNetInfo);
  server.on("/generate_204", handle_getNetInfo);
  server.on("/fwlink", handle_getNetInfo);
  server.begin();
  wifi_IP = WiFi.localIP().toString();
  Serial.println("Fatto");
  //interface = wifiCreateInter;
}

void tryConnect() {
  loadSector(1);
  if(memory_map[0] == ""){
    createNetwork();
    return;
  }
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  ret = 20;
  bool found = false;
  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.drawString(128, 52, "Scanning...");
  display.display();
  retry:
  int scanResult = WiFi.scanNetworks(/*async=*/false, /*hidden=*/true);
  Serial.print("Scan = ");
  Serial.println(scanResult);
  if (scanResult == 0) {
    ret--;
    if (!ret){
      createNetwork();
      return;
    }
    goto retry;
  }
  else if (scanResult > 0) {
    String ssid;
    int32_t rssi;
    uint8_t encryptionType;
    uint8_t* bssid;
    int32_t channel;
    bool hidden;
    for (i = 0; i < scanResult && !found; i++){
      WiFi.getNetworkInfo(i, ssid, encryptionType, rssi, bssid, channel, hidden);
      Serial.printf(PSTR("  %02d: [CH %02d] [%02X:%02X:%02X:%02X:%02X:%02X] %ddBm %c %c %s\n"),
                    i,
                    channel,
                    bssid[0], bssid[1], bssid[2],
                    bssid[3], bssid[4], bssid[5],
                    rssi,
                    (encryptionType == ENC_TYPE_NONE) ? ' ' : '*',
                    hidden ? 'H' : 'V',
                    ssid.c_str());
      for(a = 0; memory_map[a] != "" && !found; a += 2){
        if (ssid == memory_map[a]){
          found = true;
        }
      }
    }
  }
  a -= 2;
  if (found){
    ext_ssid = memory_map[a];
    ext_password = memory_map[a + 1];
    WiFi.begin(ext_ssid, ext_password);
    ret = 20;
    while (WiFi.status() != WL_CONNECTED) {
      display.clear();
      display.setFont(ArialMT_Plain_10);
      display.setTextAlignment(TEXT_ALIGN_RIGHT);
      display.drawString(128, 52, "Connecting...");
      display.display();
      delay(300);
      ret--;

      if (ret <= 0) {
        display.clear();
        display.setTextAlignment(TEXT_ALIGN_RIGHT);
        display.drawString(128, 52, "Failed!");
        display.display();
        createNetwork();
        return;
      }
    }
  }

  wifi_IP = WiFi.localIP().toString();
  //server.on("/", handle_conncetionSuccess);
  server.on("/all", handle_GetAllJson);
  server.on("/get", handle_GetSingleJson);
  server.on("/set", handle_Set);
  server.on("/delete", handle_delete);
  server.begin();
  OTAupdate();
}



/** Is this an IP? */
boolean isIp(String str) {
  for (size_t i = 0; i < str.length(); i++) {
    int c = str.charAt(i);
    if (c != '.' && (c < '0' || c > '9')) {
      return false;
    }
  }
  return true;
}

/** IP to String? */
String toStringIp(IPAddress ip) {
  String res = "";
  for (int i = 0; i < 3; i++) {
    res += String((ip >> (8 * i)) & 0xFF) + ".";
  }
  res += String(((ip >> 8 * 3)) & 0xFF);
  return res;
}
