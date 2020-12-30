//qua ficchiamo tutte le funzioni riguardanti i json



void handle_json() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", getJson());
//  server.send(200, "text/plain", "[ { \"id\":1, \"name\":\"name1\", \"username\":\"username1\", \"password\":\"password1\" }, { \"id\":2, \"name\":\"name2\", \"username\":\"username2\", \"password\":\"password2\" }  ]");
}


String getJson() {

n_section=4;
section[0].title="nome-0";
section[0].email="email-0";
section[0].password="password-0";
section[1].title="nome-1";
section[1].email="email-1";
section[1].password="password-1";
section[2].title="nome-2";
section[2].email="email-2";
section[2].password="password-2";
section[3].title="nome-3";
section[3].email="email-3";
section[3].password="password-3";
  
String  text = "[";
  for (i = 0; i < n_section; i++) {
    text += " { \"id\":";
    text += i;
    text += ", \"title\":\"";
    text += section[i].title;
    text += "\", \"email\":\"";
    text += section[i].email;
    text += "\", \"password\":\"";
    text += section[i].password;
    text += "\" }";
    if (i + 1 < n_section)text += ",";

  }
  text += "]";
  return text;
}
