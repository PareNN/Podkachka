void Izm() {
  sumP = 0.0;
  pressure = 0.0;

  // чтение и складывание значений
  for (int i = 0; i < 10; i++) {
    sumP = sumP + readVcc();
    pressure = pressure + analogRead(i_pressure);
  }


  sumP = sumP / 10.0;

  pressure = pressure / 10.0;

  //Вычисления давления 0,5-4,5  0-1.2МПа
  pressure = map(pressure, 102, 920, 0, 1200);
  pressure = pressure * 0.01;
}

// ***************************************************************************************************************************************************
// функция вычисления напряжения питания
// ***************************************************************************************************************************************************
long readVcc() {
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#elif defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
  ADMUX = _BV(MUX5) | _BV(MUX0);
#elif defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
  ADMUX = _BV(MUX3) | _BV(MUX2);
#else
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#endif

  delay(2);             // Wait for Vref to settle
  ADCSRA |= _BV(ADSC);  // Start conversion
  while (bit_is_set(ADCSRA, ADSC))
    ;  // measuring

  uint8_t low = ADCL;   // must read ADCL first - it then locks ADCH
  uint8_t high = ADCH;  // unlocks both

  long result = (high << 8) | low;

  result = 1125300L / result / 1000;  // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  return result;                      // Vcc in millivolts
}
