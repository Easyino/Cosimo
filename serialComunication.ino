void executeSerialCommands() {
  if (stringComplete) {
    Serial.println(inputString);
    if (inputString == "add") {
      
    }
    else {
      Serial.println(F("Comando non riconosciuto"));
    }
    inputString = "";
    stringComplete = false;
  }
}


void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '\n') {
      stringComplete = true;
    }
    else {
      inputString += inChar;
    }
  }
}

String extractSerialCommand(String data, int command){
  String sub_string;
  for(a = 0; a < command; a++){
    sub_string = "";
    for(i = 0; data[i] != ' '; i++){
      sub_string += data[i];
    }
  }
  return sub_string;
}
