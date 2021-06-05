void handle_getNetInfo() {
  if (captivePortal()) {
    return;
  }
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.send(200, "text/html", pgGetNetInfo());
  server.client().stop();
}

void handle_confNetInfo() {
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  loadSector(1);
  int pos;
  for(pos = 0; memory_map[pos] != ""; pos += 2){}
  Serial.print("WiFi n° ");
  Serial.println(pos);
  updateCommand(pos, server.arg("SSID"), text);
  updateCommand(pos + 1, server.arg("Password"), password);
  updateEEPROM();
  Serial.println(memory_map[0]);
  server.send(200, "text/html", "Done!");
  server.client().stop();
  tryConnect();
}

void handle_conncetionSuccess() {
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.send(200, "text/html", "you are connected succesfully");
  server.client().stop();
}

boolean captivePortal() {
  if (!isIp(server.hostHeader()) && server.hostHeader() != (String(myHostname) + ".local")) {
    Serial.println("Request redirected to captive portal");
    server.sendHeader("Location", String("http://") + toStringIp(server.client().localIP()), true);
    server.send(302, "text/plain", "");   // Empty content inhibits Content-length header so we have to close the socket ourselves.
    server.client().stop(); // Stop is needed because we sent no content length
    return true;
  }
  return false;
}