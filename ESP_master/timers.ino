void checkTimers(){
  if (millis_gap == 0 && millis_time - timer_track > 3000){
    timer_track = millis_time;
    prescaler++;
    prescaler %= 6;
    timer_trigg = prescaler + 1;
  }
  else if (timer_trigg != 0){
    timer_trigg = -1;
  }
}