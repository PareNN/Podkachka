void Stop() {
  Status = 0;
  Timer_plus.stop();
  Timer_pause.stop();
  digitalWrite(rele_plus, HIGH);
  digitalWrite(rele_minus, HIGH);
  digitalWrite(green_led, HIGH);
  digitalWrite(red_led, HIGH);
}
