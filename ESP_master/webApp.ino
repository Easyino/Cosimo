void handle_getNetInfo() {
  server.send(200, "text/html", pgGetNetInfo());
}

void handle_confNetInfo() {
  Serial.println("Qualcuno mi ha fatto una richiesta.../n");
  loadSector(1);
  for(i = 0; memory_map[i] != ""; i += 2){}
  i--;
  updateCommand(i, server.arg("SSID"), text);
  updateCommand(i + 1, server.arg("Password"), password);
  updateEEPROM();
  server.send(200, "text/html", "ORA MI RIAVVIO");
  EEPROM.write(1, 1); //set netmode to "try connect"
  EEPROM.commit();
  //demoSectors();//To remove
  ESP.restart();
}

void handle_conncetionSuccess() {
  server.send(200, "text/html", "ti sei connesso con successo alla rete");
}