/**
 * @brief Button input
 * 
 * @return The button pressed (up, down, confirm)
 */
int debouncedButtons(){
  if (digitalRead(button_up) && digitalRead(button_confirm) && digitalRead(button_down)){
    last_millis = millis();
  }
  millis_gap = millis() - last_millis; // To use it in the interfaces
  if (millis_gap > bouncing_time){
    if (!digitalRead(button_up) && int(millis_gap / 10) % (scrolling_time / 10) == 0){
      return up;
    }
    if (!digitalRead(button_confirm)){
      return confirm;
    }
    if (!digitalRead(button_down) && int(millis_gap / 10) % (scrolling_time / 10) == 0){
      return down;
    }
  }
  return -1;
}