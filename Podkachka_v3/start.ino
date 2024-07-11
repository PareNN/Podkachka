void Start() {
  if (pressure == Ustavka) Status = 0;
  if (pressure < Ustavka) {
    Status = 1;
    Timer_plus.start();
  }
  if (pressure > Ustavka) {
    Status = 2;
  }
}

void Start_test() {
  if (Timer_pause.isReady()) {
    if (pressure == Ustavka) Status = 0;
    if (pressure < Ustavka) {
      if (pressure < 0.1) Status = 0;
      else {
        Status = 1;
        Timer_plus.start();
      }
    }
    if (pressure > Ustavka) {
      Status = 2;
    }
  }
}

void Start_plus() {
  digitalWrite(rele_plus, LOW);
  if (Timer_plus.isReady()) {
    digitalWrite(rele_plus, HIGH);
    Timer_pause.start();
    Status = 3;
  }
}

void Start_minus() {
  digitalWrite(rele_minus, LOW);
  if (pressure <= Ustavka) Status = 0;
}
