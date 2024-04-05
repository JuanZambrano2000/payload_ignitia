

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  Serial.println("Se inicio el programa");
}

void loop() {
  buttonState = digitalRead(BUTTON_PIN);

  if (buttonState != lastButtonState) {
    if (buttonState == HIGH) {
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState);
      Serial.println("Se presiono el boton");
    }
    delay(50); // Debounce delay to avoid multiple toggles
  }

  lastButtonState = buttonState;
}