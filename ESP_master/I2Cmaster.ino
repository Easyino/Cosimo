/**
   @brief I2C communication

   @param data The string or the command to send
   @param type Data type (text, command, password)
*/
void sendSlave(String data, int type) {
  bool new_data = true;
  for (r = 0; data[r] != '\0'; r += a) {
    Wire.beginTransmission(I2C_SLAVE);
    Serial.println("Sending to slave:");
    for (a = 0; data[r + a] != '\0' && a < 14; a++) {
      if (new_data) {
        Wire.write(type + 1);
        new_data = false;
      }
      Wire.write(data[r + a]);
      Serial.print(data[r + a]);
    }
    Wire.write('\0');
    Wire.endTransmission();
  }
}
/**
   @brief I2c comunication

   @return int If the attiny has finished typing things
*/
int checkReady() {
  int statement = 0;
  Wire.requestFrom(I2C_SLAVE, 1);
  while (Wire.available()) {
    statement = (int)Wire.read();
  }
  return statement;
}
