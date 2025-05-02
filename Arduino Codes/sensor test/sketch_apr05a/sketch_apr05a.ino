#include <Wire.h>                      // Include the I2C communication library
#include "Adafruit_VL6180X.h"         // Include the library for the VL6180X sensor

Adafruit_VL6180X sensor = Adafruit_VL6180X(); // Create a sensor object

void setup() {
  Serial.begin(9600);               // Start the serial monitor at 115200 bps
  while (!Serial) {                   // Wait until the serial monitor is ready
    delay(1);
  }

  Serial.println("Starting VL6180X sensor...");

  // Try to connect to the sensor
  if (!sensor.begin()) {
    Serial.println("Sensor not found!"); // Print if sensor is not detected
    while (1); // Stop the program here
  }

  Serial.println("Sensor is ready!");
}

void loop() {
  // Get the distance reading from the sensor
  uint8_t distance = sensor.readRange();

  // Check the status of the reading
  uint8_t status = sensor.readRangeStatus();

  // If there's no error, print the distance
  if (status == VL6180X_ERROR_NONE) {
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" mm");
  } else {
    // If there's an error, show what kind of error it is
    if (status >= VL6180X_ERROR_SYSERR_1 && status <= VL6180X_ERROR_SYSERR_5) {
      Serial.println("System error");
    } else if (status == VL6180X_ERROR_ECEFAIL) {
      Serial.println("ECE failure");
    } else if (status == VL6180X_ERROR_NOCONVERGE) {
      Serial.println("No convergence");
    } else if (status == VL6180X_ERROR_RANGEIGNORE) {
      Serial.println("Ignoring range");
    } else if (status == VL6180X_ERROR_SNR) {
      Serial.println("Signal/Noise error");
    } else if (status == VL6180X_ERROR_RAWUFLOW) {
      Serial.println("Raw reading underflow");
    } else if (status == VL6180X_ERROR_RAWOFLOW) {
      Serial.println("Raw reading overflow");
    } else if (status == VL6180X_ERROR_RANGEUFLOW) {
      Serial.println("Range reading underflow");
    } else if (status == VL6180X_ERROR_RANGEOFLOW) {
      Serial.println("Range reading overflow");
    }
  }

  delay(500); // Wait for 2 seconds before taking the next reading
}
