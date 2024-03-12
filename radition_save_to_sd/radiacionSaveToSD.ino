/*
  Based on initial work of Mark A. Heckler and Andreas Spiess
  Author: Juan Zambrano (a01636420) 
  Author: Stefano Herrejon (A00571720)
  Please use freely with attribution. Thank you!
*/

// SD imports
#include <SPI.h>
#include <SdFat.h>

SdFat sd;                                                // Create an sd object
const int chipSelect = 4;                                // CS pin of the sd card reader

// Geiger counter variables
volatile unsigned long counts = 0;                       // Tube events
unsigned long cpm = 0;                                   // CPM
unsigned long previousMillis;                            // Time measurement
const int inputPin = 2;                                  // Geiger counter is in this pin
unsigned long minutes = 1;
unsigned long start = 0;
volatile unsigned long totalCounts = 0;
bool finish = false;

#define LOG_PERIOD 1000 // Logging period in milliseconds
#define MINUTE_PERIOD 10000

//Duracion de las medidas, se usa para cerrar el archivo de forma segura
#define MAX_DURATION 30000 // Time of the experiment - 10 seconds

SdFile file; // File object

// Interrupt Service Routine (ISR)
void ISR_impulse() {
  counts++;
  totalCounts++;
}

void setup() {
  Serial.begin(9600); // Initialize serial communication
  while (!Serial) {
    ; // Wait for Serial port to be available
  }

  // Initialize the SD card
  if (!sd.begin(chipSelect)) {
    sd.initErrorHalt();
    // Program execution will halt here if sd.begin() fails
    // and print the error message
  }

  Serial.println("SD card initialization successful!");

  // Open the file for writing
  file.open("readings.txt", FILE_WRITE);

  if (!file.isOpen()) {
    Serial.println("Error opening file");
    return;
  }else{
    file.println("/////////////////////////////////////////////");
    file.println("Rolling CPM every ");
    file.println(LOG_PERIOD);
    file.println("miliseconds");
  }

  delay(1000); // Wait for stability
  Serial.println("Booting..."); // Print boot message
  Serial.println("Measuring"); // Print measurement message

  pinMode(inputPin, INPUT); // Set pin for capturing tube events
  interrupts(); // Enable interrupts
  attachInterrupt(digitalPinToInterrupt(inputPin), ISR_impulse, FALLING); // Define interrupt on falling edge

  start = millis(); // Timer
}

void loop() {
  unsigned long currentMillis = millis(); // Get current time

  if (currentMillis - previousMillis > LOG_PERIOD && !finish) {
    previousMillis = currentMillis; // Update previous time

    minutes = (previousMillis - start) / MINUTE_PERIOD; // Calculate elapsed minutes

    if (minutes < 1) {
      minutes = 1; // Ensure a minimum of 1 minute
    }

    cpm = counts / minutes; // Calculate counts per minute (CPM)
    Serial.println("Rolling CPM every 0.5 seconds: ");
    Serial.println(String(cpm));

    if (file.isOpen()) {
      file.println(String(cpm));
    } else {
      Serial.println("Error writing to file");
    }

    counts = 0; // Reset counts for the next period
  }

  // Close the file after 10 minutes
  if (currentMillis - start >= MAX_DURATION) {
    if (!finish){
      file.print("Total clicks since start: ");
      file.println(String(totalCounts));
      file.print("Experiment finished, total time : ");
      file.println(MAX_DURATION);
      file.close(); // Close the file
      Serial.println("Experiment finished, total time : ");
      Serial.println(MAX_DURATION);
    }
    finish = true;
    delay(10000);
  }
}