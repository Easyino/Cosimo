//qua ficchiamo tutte le funzioni riguardanti i json



void handle_GetAllJson() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", getAllJson());
  //  server.send(200, "text/plain", "[ { \"id\":1, \"name\":\"name1\", \"username\":\"username1\", \"password\":\"password1\" }, { \"id\":2, \"name\":\"name2\", \"username\":\"username2\", \"password\":\"password2\" }  ]");
}

void handle_GetSingleJson() {
  server.sendHeader("Access-Control-Allow-Origin", "*");

  server.send(200, "text/plain", getSingleJson(server.arg("id").toInt()));
}

void handle_delete() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  Serial.print("è stato eliminato l'elemento: ");
  Serial.println( server.arg("id").toInt());
  server.send(200, "text/plain", "1 element deleted");
}

void handle_Set() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
if(server.arg("id").toInt()!=0){
  Serial.print("è stato modificato l'elemento: ");
  Serial.println( server.arg("id").toInt());
  }
  else{
     Serial.println("è stato creato un nuovo elemento:");
  }

  Serial.print("title: ");
  Serial.println( server.arg("title"));

  Serial.print("email: ");
  Serial.println( server.arg("email"));

  Serial.print("password: ");
  Serial.println( server.arg("password"));

  server.send(200, "text/plain", server.arg("id") );
}

String getAllJson() {

  String  text = "[";
  for (i = 0; i < n_section; i++) {
    text += " { \"id\":";
    text += i;
    text += ", \"title\":\"";
    text += section[i].title;
    text += "\", \"email\":\"";
    text += section[i].email;
    text += "\" }";
    if (i + 1 < n_section)text += ",";

  }
  text += "]";
  return text;
}
String getSingleJson(int id) {


   String text = " { \"id\":";
    text += id;
    text += ", \"title\":\"";
    text += section[id].title;
    text += "\", \"email\":\"";
    text += section[id].email;
        text += "\", \"password\":\"";
    text += section[id].password;
    text += "\" }";
  

  return text;
}
