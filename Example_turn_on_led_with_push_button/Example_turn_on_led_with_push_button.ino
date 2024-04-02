#define LED_PIN 8
#define BUTTON_PIN 7

bool ledState = false;
bool buttonState = false;
bool lastButtonState = false;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
}

void loop() {
  buttonState = digitalRead(BUTTON_PIN);

  if (buttonState != lastButtonState) {
    if (buttonState == HIGH) {
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState);
    }
    delay(50); // Debounce delay to avoid multiple toggles
  }

  lastButtonState = buttonState;
}