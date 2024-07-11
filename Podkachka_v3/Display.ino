
void Display() {
  lcd.print(Ustavka+0.001, 1);
  // Монитор порта
  Serial.print("Ustavka ");
  Serial.print(Ustavka * 0.1);
  Serial.print(" Давление ");
  Serial.print(pressure);
  Serial.print(" Режим - ");
  if (Status == 0) Serial.println("Стоп");
  if (Status == 1) Serial.println("/\\");
  if (Status == 2) Serial.println("\\/");
  if (Status == 3) Serial.println("****");
}
