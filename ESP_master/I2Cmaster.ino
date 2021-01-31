/**
 * @brief I2C communication
 * 
 * @param data The string or the command to send
 * @param type Data type (text, command, password)
 */
void sendSlave(String data, int type) {
  for (r = 0; data[r] != '\0'; r += a) {
    Wire.beginTransmission(I2C_SLAVE);
    for (a = 0; data[r + a] != '\0' && a < 15; a++) {
      if (r == 0 && a == 0){
        Wire.write(type);
      }
      else{
        Wire.write(data[r + a - 1]);
      }
    }
    Wire.endTransmission();
  }
}
/**
 * @brief I2c comunication
 * 
 * @return int If the attiny has finished typing things
 */
int checkReady() {
  int statement = 0;
  Wire.requestFrom(I2C_SLAVE, 1);
  while (Wire.available()) {
    statement = (int)Wire.read();
  }
  return statement;
}
