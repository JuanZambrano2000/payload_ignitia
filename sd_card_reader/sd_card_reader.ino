#include <SPI.h>
#include <SdFat.h>

// Create an instance of the SdFat class
SdFat sd;

// Chip Select pin for the SD card module
const int chipSelect = 4;

void setup() {
  Serial.begin(9600);
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

  // Create or open the file "test.txt" for writing
  SdFile file("test.txt", FILE_WRITE);

  // Check if the file was opened successfully
  if (file.isOpen()) {
    Serial.println("File opened successfully!");

    // Write "hello world" to the file
    file.println("hello world");

    // Close the file
    file.close();
    Serial.println("File closed successfully.");
  } else {
    sd.errorHalt("Failed to open file");
    // Program execution will halt here if file opening fails
  }
}

void loop() {
  // Your main program loop
}