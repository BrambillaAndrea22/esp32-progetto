const int PIN_LED = 26;
const unsigned long tempo_acceso_ms = 1000;
const unsigned long tempo_spento_ms = 1000;

bool ledAcceso = false;
unsigned long ultimoCambio_ms = 0;

void setup() {
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);
}
void loop() {
  unsigned long clock = millis();
  unsigned long durata;

  if (ledAcceso)
    durata = tempo_acceso_ms;
  else 
    durata = tempo_spento_ms;

  if (clock - ultimoCambio_ms >= durata) {
    ledAcceso = !ledAcceso;   
    if (ledAcceso) 
      digitalWrite(PIN_LED, HIGH);
    else 
      digitalWrite(PIN_LED, LOW);
    ultimoCambio_ms = clock;
  }
}