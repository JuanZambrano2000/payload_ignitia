/*
  Based on initial work of Mark A. Heckler and Andreas Spiess
  Author: Juan Zambrano (a01636420) 
  Author: Stefano Herrejon (A00571720)
  Please use freely with attribution. Thank you!
*/

//SD //////////////////////////////////////////////////////
// SD imports
#include <SPI.h>
#include <SdFat.h>

SdFile file;                                             // File object
SdFat sd;                                                // Create an sd object
const int chipSelect = 4;                                // CS pin of the sd card reader

// Geiger counter /////////////////////////////////////////
volatile unsigned long counts = 0;                       // Tube events
const int inputPin = 2;                                  // Geiger counter is in this pin
volatile unsigned long totalCounts = 0;


// EXPERIMENT ////////////////////////////////////////////
#define LOG_PERIOD 500                                  // Logging period in milliseconds 0.5 seconds
#define MAX_DURATION 28800000                              // Duracion de las medidas, se usa para cerrar el archivo de forma segura
#define SAVE_DURATION 60000                             // Time for each save of the document

unsigned long previousMillisSave;                        //How muchtime, needs to be adjusteded to the same as SAVE_DURATION
unsigned long previousMillis;                            // Time measurement
unsigned long start = 0;                                 // Star in miliseconds
bool finish = false;                                     //flag for the end of the experiment
unsigned long recordNumber = 0;

//BUTTON & LED ////////////////////////////////////////////
#define LED_PIN 8
#define BUTTON_PIN 7

bool ledState = false;
bool buttonState = false;
bool lastButtonState = false;
bool boton = false;

// Interrupt Service Routine (ISR)
void ISR_impulse() {
  counts++;
}

// Open and close the document to avoid losing data in case of sudden disconnection
void open_close(){
    // Open the file for writing
  file.close();
  file.open("readings.txt", FILE_WRITE);
  Serial.println("Open Close /////////////////////////////////////////////");

  if (!file.isOpen()) {
    Serial.println("Error opening file");
    return;
  }
  else{
    file.println("-----------------------------------------");
  }
  
}

void setup() {
  // Initialize serial communication
  Serial.begin(9600); 
  while (!Serial) {
    ; // Wait for Serial port to be available
  }

  //SD card
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

  //Geiger counter
  pinMode(inputPin, INPUT); // Set pin for capturing tube events
  interrupts(); // Enable interrupts
  attachInterrupt(digitalPinToInterrupt(inputPin), ISR_impulse, FALLING); // Define interrupt on falling edge

  //Experiment
  start = millis(); // Timer
  Serial.println("Booting..."); // Print boot message
  Serial.println("Measuring"); // Print measurement message

  //LED & BUTTOM
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
}

void loop() {

  if (buttonState != lastButtonState) {
    if (buttonState == HIGH) {
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState);
      Serial.println("Se presiono el boton");
      file.println("---------------BOTON------------------");
      boton = !boton;
    }
    delay(50); // Debounce delay to avoid multiple toggles
    counts = 0;
  }

  lastButtonState = buttonState;
  buttonState = digitalRead(BUTTON_PIN);

  //Experiment, measurements and records
  unsigned long currentMillis = millis(); // Get current time

  if (currentMillis - previousMillis > LOG_PERIOD && !finish && boton) { // Check if the measurement time ei up, the time has not finiched and if the button has been pressed
    previousMillis = currentMillis; // Update previous time

    Serial.print("Counts :  ");
    Serial.print(String(counts));
    Serial.println("");

    if (file.isOpen()) {
      file.print(recordNumber);
      file.print(",");
      file.print(String(counts));
      file.println("");
    } else {
      Serial.println("Error writing to file");
    }

    if (currentMillis - previousMillisSave > SAVE_DURATION) {//Check if the save duration has run out
      previousMillisSave = currentMillis;
      open_close();
    }

    totalCounts = totalCounts + counts;
    recordNumber++;
    counts = 0; // Reset counts for the next period

  }

  // Stop the experiment after the time runs out
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
  }

}
