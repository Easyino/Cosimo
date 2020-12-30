void sendSector() {
  Wire.beginTransmission(I2C_SLAVE);
  for (i = 0; memory_map[i] != ""; i++) {
    for(a = 0; memory_map[i][a] != '\0'; a++){
      Wire.write(memory_map[i][a]);
    }
  }
  Wire.endTransmission();
}

int reciveState() {
  int statement = 0;
  Wire.requestFrom(I2C_SLAVE, 1);
  while (Wire.available()) { // slave may send less than requested
    statement = Wire.read(); // receive a byte as character
  }
  return statement;
}
