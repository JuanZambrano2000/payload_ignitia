/*
  Based on initial work of Mark A. Heckler and Andreas Spiess 
   Author: Juan Zambrano (a01636420)
   Author: Stefano Herrejon
   Please use freely with attribution. Thank you!
*/

//Sd imports
#include <SPI.h>
#include <SdFat.h>
SdFat sd;                                                // Create an sd object
const int chipSelect = 4;                                // CS pin
// Geiger counter import
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
  while (!Serial) {
    ; // Wait for Serial port to be available
  }

  if (!sd.begin(chipSelect)) { //wait for sd to start
    sd.initErrorHalt();
    // Program execution will halt here if sd.begin() fails
    // and print the error message
  }

  // Create or open the file "test.txt" for writing
  SdFile file("readings.txt", FILE_WRITE);

  Serial.println("SD card initialization successful!");

  // Open the file for writing
  SdFile file("readings.txt", O_RDWR | O_CREAT | O_AT_END);
  if (!file.isOpen()) {
      Serial.println("Error opening file");
      return;
  }

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
    file.print("Total clicks since start: ");
    file.println(String(totalCounts));
    file.print("Rolling CPM: ");
    file.println(String(cpm));
    file.flush(); // Ensure data is written to the file
    counts = 0; // Reset counts for the next period
  }
  if (minutes > 10 ){
    File.close();
  }
}

  
  
  