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
int checkReady() {
  int statement = 0;
  Wire.requestFrom(I2C_SLAVE, 1);
  while (Wire.available()) {
    statement = (int)Wire.read();
  }
  return statement;
}
