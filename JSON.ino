//qua ficchiamo tutte le funzioni riguardanti i json



void handle_json() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", getJsonPasswords());
}


String getJsonPasswords() {
String  text = "[";
  for (i = 0; checkpoint_memory[sector_loaded] != 0; i++) {
    loadSector(i);
    text += " { \"id\":";
    text += i;
    text += ", \"title\":\"";
    text += memory_map[0];
    text += "\", \"email\":\"";
    text += memory_map[1];
    text += "\", \"password\":\"";
    text += memory_map[2];
    text += "\" }";
    if (checkpoint_memory[sector_loaded + 1] != 0)text += ",";
  }
  text += "]";
  return text;
}
