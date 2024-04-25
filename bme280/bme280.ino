#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1015)
#define TARGET_ALTITUDE_OFFSET 5 // Offset to add to the initial altitude (in meters)

Adafruit_BME280 bme;
float initialAltitude;
float targetAltitude;
bool initialAltitudeSet = false;
bool measurementStarted = false;
bool measurementStopped = false;

void setup() {
  Serial.begin(9600);
  Serial.println("test1");
  if (!bme.begin(0x76)) {
    Serial.println("No encuentro un sensor BME280 valido!");
    while (1);
  }
  Serial.println("test2");
  delay(50);
  float initialAltitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
  float targetAltitude = initialAltitude + TARGET_ALTITUDE_OFFSET;
  Serial.print("Altitud Inicial : ");
  Serial.println(initialAltitude);
  Serial.println("Altitud donde se para de medir: ");
  Serial.println(targetAltitude);
}

void loop() {
  float currentAltitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
  
  // Set the initial altitude using the first measurement and add the offset
  if (!initialAltitudeSet) {
    initialAltitude = currentAltitude + TARGET_ALTITUDE_OFFSET;
    initialAltitudeSet = true;
    Serial.print("Altitud inicial establecida: ");
    Serial.print(initialAltitude);
    Serial.println("m");
  }
  
  // Check if the rocket has reached the target altitude
  if (currentAltitude >= initialAltitude && !measurementStarted) {
    measurementStarted = true;
    Serial.println("Mediciones iniciadas.");
  }
  
  // Check if the rocket has reached the target altitude for the second time
  if (measurementStarted && currentAltitude <= initialAltitude && !measurementStopped) {
    measurementStopped = true;
    Serial.println("Altura alcanzada, deteniendo mediciones.");
  }
  /*
  // Print the sensor data
  Serial.print("Temperatura = ");
  Serial.print(bme.readTemperature());
  Serial.println("*C");
  
  Serial.print("Presion = ");
  Serial.print(bme.readPressure() / 100.0F);
  Serial.println("hPa");
  
  Serial.print("Altitud estimada = ");
  Serial.print(currentAltitude);
  Serial.println("m");
  
  Serial.print("Humedad = ");
  Serial.print(bme.readHumidity());
  Serial.println("%");
  
  Serial.println();
  */
  
  delay(50);
}