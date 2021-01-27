int debouncedButtons(){
  if (digitalRead(button_up) == HIGH && digitalRead(button_up) == HIGH && digitalRead(button_up) == HIGH){
    last_millis = millis();
  }
  millis_gap = millis() - last_millis + 1; // To use it in the interfaces
  if (millis_gap > 30){
    if (digitalRead(button_up) == LOW){
      return up;
    }
    if (digitalRead(button_confirm) == LOW){
      return confirm;
    }
    if (digitalRead(button_down) == LOW){
      return down;
    }
  }
  return -1;
}
