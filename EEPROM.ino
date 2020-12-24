
/*
void writeString(char add,String data){
  int _size = data.length();
  int i;
  for(i=0;i<_size;i++)
  {
    EEPROM.write(add+i,data[i]);
  }
  EEPROM.write(add+_size,'\0');   
  EEPROM.commit();
}
*/

String read_String(char add){
  int i;
  char data[100]; //Max 100 Bytes
  int len=0;
  unsigned char k;
  k=EEPROM.read(add);
  while(k != '\0' && len<500)   //Read until null character
  {    
    k=EEPROM.read(add+len);
    data[len]=k;
    len++;
  }
  data[len]='\0';
  return String(data);
}

void loadNetData(){ 
if(EEPROM.read(1)!=0){
netMode=1;
   ext_ssid = newReadString(addrExtSSID);
 Serial.println("prendo SSID");
 ext_password = newReadString(addrExtPassword); 
  Serial.println("prendo password");
}
netMode=0;
}



void newWriteString(int add, String data) {
  int _size = data.length();
  int pos = 0;
  for (int sec = 0; sec < add; sec++) {
    pos = pos + EEPROM.read(pos);
  }
  EEPROM.write(pos, _size + 1);
  Serial.print("scrivo: ");
  Serial.print( _size + 1);
  Serial.println(" in posizione: ");
  Serial.println(pos);

  pos++;
  for (int i = 0; i < _size; i++) {
    EEPROM.write(pos + i, data[i]);

    Serial.print("scrivo: ");
    Serial.print( data[i]);
    Serial.println(" in posizione: ");
    Serial.println(pos + i);
  }

  EEPROM.commit();
}

String newReadString(int add) {
  char data[100];
  int pos = 0, length;
  unsigned char l;
  for (int sec = 0; sec < add; sec++) {
    pos = pos + EEPROM.read(pos);
  }
  length = EEPROM.read(pos) - 1;
  Serial.print("La lunghezza è: ");
  Serial.println( length);

  pos++;
  for (int i = 0; i < length; i++) {

    data[i] = EEPROM.read(pos + i);
    Serial.print("leggo: ");
    Serial.print( data[i]);
    Serial.print(" in posizione: ");
    Serial.println(i);
  }
  data[length] = '\0';

  Serial.println(String(data));
  return String(data);
}
