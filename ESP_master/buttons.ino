/**
 * @brief Button input
 * 
 * @return The button pressed (up, down, confirm)
 */
bool confirmed = false;
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
      if (!confirmed) {
        confirmed = true;
        return confirm;
      }
    }
    else if (confirmed){
      confirmed = false;
    }
    if (!digitalRead(button_down) && int(millis_gap / 10) % (scrolling_time / 10) == 0){
      return down;
    }
  }
  return -1;
}