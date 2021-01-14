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
  Serial.println( server.arg("n_seq").toInt());
    Serial.println( server.arg("n_id").toInt());
  server.send(200, "text/plain", "1 element deleted");
}

void handle_Set() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  if (server.arg("id").toInt() != 0) {
    Serial.print("è stato modificato l'elemento: ");
    Serial.println( server.arg("id").toInt());
  }
  else {
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

  String  tuaNonna = "[";
  for (i = 0; i < n_section; i++) {
    getSingleJson(i);
    if (i + 1 < n_section)tuaNonna += ",";
  }
  tuaNonna += "]";
  return tuaNonna;
}


String getSingleJson(int id) {

  struct box {
    String type;
    String content;
  };
  struct box box[10];
  int n_box = 3;




  box[0].type = "text";
  box[0].content = "testo1";
  box[1].type = "password";
  box[1].content = "pass1";
  box[2].type ="text";
  box[2].content = "testo2";
  
  String  text = "[";

  for (i = 0; i < n_box; i++) {
    text += " { \"n_seq\":";
    text += i;
    text += ", \"type\":\"";
    text += box[i].type;
    text += "\", \"content\":\"";
    text += box[i].content;
    text += "\" }";
    if (i + 1 < n_box)text += ",";

  }
  text += "]";
}
