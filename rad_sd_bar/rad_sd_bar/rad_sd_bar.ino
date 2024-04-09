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
#define LOG_PERIOD 500                                  // Logging period in milliseconds -> 0.5 seconds
#define MAX_DURATION 300000                              // Duracion de las medidas, se usa para cerrar el archivo de forma segura
#define SAVE_DURATION 10000                             // Time for each save of the document

unsigned long previousMillisSave;                        //How muchtime, needs to be adjusteded to the same as SAVE_DURATION
unsigned long previousMillis;                            // Time measurement
unsigned long start = 0;                                 // Star in miliseconds
bool finish = false;                                     //flag for the end of the experiment
unsigned long recordNumber = 0;

//BUTTON & LED ////////////////////////////////////////////
#define LED_PIN 9
#define BUTTON_PIN 8

bool ledState = false;
bool buttonState = false;
bool lastButtonState = false;
bool boton = false;

// Barometro (bme280) ////////////////////////////////////////
//Barometro imports
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1015)

Adafruit_BME280 bme;
float initialAltitude;


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
    //file.println("-----------------------------------------");
    //No se muestra en el archivo cuando ocurrio esta acción
  }
  
}

void setup() {
  // Initialize serial communication
  Serial.begin(9600); 
  while (!Serial) {
    ; // Wait for Serial port to be available
  }

  //Barometro 
  if (!bme.begin(0x76)) {
    Serial.println("No encuentro un sensor BME280 valido!");
    while (1);
  }
  float initialAltitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
  float temperaturaInicial = bme.readTemperature();

  delay(50);

  //SD card
  if (!sd.begin(chipSelect)) {
    sd.initErrorHalt();
    // Program execution will halt here if sd.begin() fails

  }else{
    Serial.println("SD card initialization successful!");
    file.open("readings.txt", FILE_WRITE);  // Open the file for writing
  }

  if (!file.isOpen()) {
    Serial.println("Error opening file");
    return;
  }else{
    file.println("/////////////////////////////////////////////");
    file.println("Experiment : ");
    file.println("Format : Count, Altitud, Temperatura, Rad.");
    file.print("Total time : ");
    file.print(MAX_DURATION);
    file.println("  Miliseconds");
    file.print("Measurements every : ");
    file.print(LOG_PERIOD);
    file.println("  Miliseconds");
    file.print("Save Data every : ");
    file.print(SAVE_DURATION);
    file.println("  Miliseconds");

    file.print("Altura inicial : ");
    file.println(initialAltitude);
    file.print("Temperatura inicial : ");
    file.println(temperaturaInicial);
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
  Serial.print("Altitud Inicial : ");
  Serial.println(initialAltitude);
  Serial.print("Temperatura Inicial : ");
  Serial.println(temperaturaInicial);


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
  float currentAltitude = bme.readAltitude(SEALEVELPRESSURE_HPA); //Altitude
  float currentTemp = bme.readTemperature();


  if (currentMillis - previousMillis > LOG_PERIOD && !finish && boton) { // Check if the measurement time ei up, the time has not finiched and if the button has been pressed
    previousMillis = currentMillis; // Update previous time

    Serial.print("Counts :  ");
    Serial.print(String(counts));
    Serial.println("");
    Serial.print("Altitud  :  ");
    Serial.println(currentAltitude);
    Serial.print("Tepmperatura  :  ");
    Serial.println(currentTemp);
    Serial.println("");

    if (file.isOpen()) {
      file.print(recordNumber);
      file.print(",");
      file.print(String(currentAltitude));
      file.print(",");
      file.print(String(currentTemp));
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
