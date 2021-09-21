/**
   @brief I2C communication

   @param data The string or the command to send
   @param type Data type (text, command, password)
*/
void endSlaveBuffering() {
  Wire.beginTransmission(I2C_SLAVE);
  Wire.write(254);
  Wire.endTransmission();
}

void sendSlave(byte type, String data) {
  int i, a, r;
  data = char(type) + data + char(125);
  for (r = 0; data[r] != '\0'; r += a) {
    Wire.beginTransmission(I2C_SLAVE);
    for (a = 0; data[r + a] != '\0' && a < 14; a++) {
      Wire.write(data[r + a]);
      Serial.print(data[r + a]);
    }
    Wire.endTransmission();
  }
}

void sendSector() {
  int i;
  Serial.println("Sending to slave:");
  for (i = 1; memory_map[i] != ""; i++) {
    sendSlave(memory_type[i], (memory_map[i + 1] == "") ? (memory_map[i] + char(126)) : memory_map[i]);
  }
}
