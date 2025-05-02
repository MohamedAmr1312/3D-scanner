#include <Wire.h>
#include "Adafruit_VL6180X.h"

Adafruit_VL6180X sensor = Adafruit_VL6180X();

// Manually set your position (change it before each reading)
float posX = 0.0;
float posY = 0.0;
float posZ = 0.0;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  
  if (!sensor.begin()) {
    Serial.println("Failed to find sensor");
    while (1);
  }

  Serial.println("Sensor ready!");
}

void loop() {
  uint8_t distance = sensor.readRange();
  uint8_t status = sensor.readRangeStatus();

  if (status == VL6180X_ERROR_NONE) {
    // Calculate new point (this assumes sensor looks along Z-axis)
    float x = posX;
    float y = posY;
    float z = posZ + distance; // Adjust if your orientation is different

    // Output the 3D point
    Serial.print(x, 2); Serial.print(" ");
    Serial.print(y, 2); Serial.print(" ");
    Serial.println(z, 2);
  } else {
    Serial.println("Sensor error");
  }

  delay(2000); // Wait 2 seconds between readings
}
