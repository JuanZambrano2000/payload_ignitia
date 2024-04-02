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
unsigned long previousMillis;                            // Time measurement
unsigned long previousMillisSave;                        //How muchtime, needs to be adjusteded to the same as SAVE_DURATION
const int inputPin = 2;                                  // Geiger counter is in this pin
unsigned long start = 0;
volatile unsigned long totalCounts = 0;
bool finish = false;

#define LOG_PERIOD 500 // Logging period in milliseconds
#define MAX_DURATION 120000 // Duracion de las medidas, se usa para cerrar el archivo de forma segura
#define SAVE_DURATION 30000 // Time for each save of the document

SdFile file; // File object

// Interrupt Service Routine (ISR)
void ISR_impulse() {
  counts++;
  totalCounts++;
}

void open_close(){
    // Open the file for writing
  file.close();
  file.open("readings.txt", FILE_WRITE);
  Serial.println("Open Close");

  if (!file.isOpen()) {
    Serial.println("Error opening file");
    return;
  }else{
    file.println("-----");
  }
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

  file.open("readings.txt", FILE_WRITE);  // Open the file for writing

  if (!file.isOpen()) {
    Serial.println("Error opening file");
    return;
  }else{
    file.println("/////////////////////////////////////////////");
    file.println("RAD Count every : ");
    file.println(LOG_PERIOD);
    file.println("Miliseconds");
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

  if (currentMillis - previousMillis > LOG_PERIOD && !finish) { // Ver si ha pasado el tiempo de medición
    previousMillis = currentMillis; // Update previous time


    Serial.println("Counts :  ");
    Serial.println(String(counts));

    if (file.isOpen()) {
      file.println(String(counts));
    } else {
      Serial.println("Error writing to file");
    }

    if (currentMillis - previousMillisSave > SAVE_DURATION) {//Ver si ha pasado el tiempo para cerrar y abrir el archivo de manera segura
      previousMillisSave = currentMillis;
      open_close();
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
