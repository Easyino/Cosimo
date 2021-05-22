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
  EEPROM.write(1, 1); //set netmode to "try connect"
  EEPROM.commit();
  //demoSectors();//To remove
  ESP.restart();
}

void handle_conncetionSuccess() {
  server.send(200, "text/html", "ti sei connesso con successo alla rete");
}