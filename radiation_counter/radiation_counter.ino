/* 
   edited by genewitch to actually provide clicks per minute on the serial port
   in a sane fashion. the original file i forked from just set CPM to 105 and 
   didn't really work anyhow, as the count was reset every 20 seconds.
   There's an issue with my understanding of the way variables work, so please
   feel free to fix my cludge with the clock1 and start variables. when they 
   were in the loop the "minutes" never changed.

   trying to create PR
*/

/*
   Geiger.ino

   This code interacts with the Alibaba RadiationD-v1.1 (CAJOE) Geiger counter board
   and reports readings in CPM (Counts Per Minute).

   Author: Andreas Spiess

   Based on initial work of Mark A. Heckler (@MkHeck, mark.heckler@gmail.com)

   License: MIT License

   Please use freely with attribution. Thank you!
*/

volatile unsigned long counts = 0;                       // Tube events
unsigned long cpm = 0;                                   // CPM
unsigned long previousMillis;                            // Time measurement
const int inputPin = 2;                                  // Geiger counter is in this pin
unsigned int thirds = 0;
unsigned long minutes = 1;
unsigned long start = 0;

#define LOG_PERIOD 5000 // Logging period in milliseconds
#define MINUTE_PERIOD 60000

void ISR_impulse() { // Captures count of events from Geiger counter board
  counts++;
}

void setup() {
  // Set up the Arduino environment (runs once during startup)
  Serial.begin(9600); // Initialize serial communication
  delay(500); // Wait for stability
  Serial.println("Booting..."); // Print boot message
  Serial.println("Measuring"); // Print measurement message
  pinMode(inputPin, INPUT); // Set pin for capturing tube events
  interrupts(); // Enable interrupts
  attachInterrupt(digitalPinToInterrupt(inputPin), ISR_impulse, FALLING); // Define interrupt on falling edge
  unsigned long clock1 = millis(); // Record current time
  start = clock1; // Set start time
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
    Serial.println(String(counts)); // Print total counts
    Serial.print("Rolling CPM: ");
    Serial.println(String(cpm)); // Print rolling CPM
    counts = 0; // Reset counts for the next period
  }
}

  
  
  