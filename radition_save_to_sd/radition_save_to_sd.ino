/*
  Based on initial work of Mark A. Heckler and Andreas Spiess 
   Author: Juan Zambrano (a01636420)
   Author: Stefano Herrejon
   Please use freely with attribution. Thank you!
*/

volatile unsigned long counts = 0;                       // Tube events
unsigned long cpm = 0;                                   // CPM
unsigned long previousMillis;                            // Time measurement
const int inputPin = 2;                                  // Geiger counter is in this pin
unsigned long minutes = 1;
unsigned long start = 0;
volatile unsigned long totalCounts = 0;

#define LOG_PERIOD 500 // Logging period in milliseconds
#define MINUTE_PERIOD 60000

void ISR_impulse() { // Captures count of events from Geiger counter board
  counts++;
  totalCounts++;
}

void setup() {
  // Set up the Arduino environment (runs once during startup)
  Serial.begin(9600); // Initialize serial communication
  delay(1000); // Wait for stability
  Serial.println("Booting..."); // Print boot message
  Serial.println("Measuring"); // Print measurement message
  pinMode(inputPin, INPUT); // Set pin for capturing tube events
  interrupts(); // Enable interrupts
  attachInterrupt(digitalPinToInterrupt(inputPin), ISR_impulse, FALLING); // Define interrupt on falling edge
}

void loop() {
  // Main loop (runs repeatedly)
  unsigned long currentMillis = millis(); // Get current time
  
  if (currentMillis - previousMillis > LOG_PERIOD) {
    // If the specified logging period has elapsed:
    previousMillis = currentMillis; // Update previous time
    minutes = (previousMillis - start) / MINUTE_PERIOD; // Calculate elapsed minutes
    if (minutes < 1) {
      minutes = 1; // Ensure a minimum of 1 minute
    }
    cpm = counts / minutes; // Calculate counts per minute (CPM)
    Serial.print("Total clicks since start: ");
    Serial.println(String(totalCounts)); // Print total counts
    Serial.print("Rolling CPM: ");
    Serial.println(String(cpm)); // Print rolling CPM
    counts = 0; // Reset counts for the next period
  }
}

  
  
  