#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1015)

Adafruit_BME280 bme;
float initialAltitude;

void setup() {
  Serial.begin(9600);
  if (!bme.begin(0x76)) {
    Serial.println("No encuentro un sensor BME280 valido!");
    while (1);
  }
  delay(50);

  float initialAltitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
  Serial.print("Altitud Inicial : ");
  Serial.println(initialAltitude);
}

void loop() {
  float currentAltitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
  Serial.println(currentAltitude);
  delay(50);
}
