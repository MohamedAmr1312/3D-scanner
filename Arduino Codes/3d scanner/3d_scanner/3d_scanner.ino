#include <Wire.h>
#include "Adafruit_VL6180X.h"
#include <SD.h>
#include <SPI.h>

// Stepper motor pins
#define STEP_PIN_1 9
#define DIR_PIN_1 8
#define STEP_PIN_2 7
#define DIR_PIN_2 6

// SD card
#define SD_CS_PIN 10

// Stepper parameters
#define STEPS_PER_REV 200       // Full steps per revolution
#define STEP_ANGLE 1.8          // Step angle in degrees
#define DEGREE_STEP 5           // Rotate every 5 degrees
#define STEPS_PER_DEG (STEPS_PER_REV / 360.0)
#define STEPS_PER_STEP (int)(DEGREE_STEP * STEPS_PER_DEG)

// Lead screw
#define LEAD_SCREW_PITCH 2.0    // mm per revolution
#define STEPS_PER_MM (STEPS_PER_REV / LEAD_SCREW_PITCH)

// Delay
#define STEP_DELAY_US 1500

// Sensor setup
Adafruit_VL6180X sensor = Adafruit_VL6180X();

// Tracking
float currentZ = 0;
int rotationAngle = 0;

File dataFile;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // Stepper setup
  pinMode(STEP_PIN_1, OUTPUT);
  pinMode(DIR_PIN_1, OUTPUT);
  pinMode(STEP_PIN_2, OUTPUT);
  pinMode(DIR_PIN_2, OUTPUT);
  digitalWrite(DIR_PIN_1, HIGH); // CW for rotating table
  digitalWrite(DIR_PIN_2, HIGH); // UP for lead screw

  // Sensor init
  if (!sensor.begin()) {
    Serial.println("VL6180X not found");
    while (1);
  }
  Serial.println("Sensor ready!");

  // SD card init
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("SD init failed!");
    while (1);
  }
  Serial.println("SD card ready");

  // Open file
  dataFile = SD.open("scan.xyz", FILE_WRITE);
  if (!dataFile) {
    Serial.println("Error opening scan.xyz");
    while (1);
  }
}

void loop() {
  // Read distance
  uint8_t distance = sensor.readRange();
  uint8_t status = sensor.readRangeStatus();

  if (status == VL6180X_ERROR_NONE) {
    float r = (float)distance;             // mm
    float thetaRad = radians(rotationAngle);

    float x = r * cos(thetaRad);
    float y = currentZ;
    float z = r * sin(thetaRad);

    // Print to serial
    Serial.print(x, 2); Serial.print(" ");
    Serial.print(y, 2); Serial.print(" ");
    Serial.println(z, 2);

    // Write to SD
    if (dataFile) {
      dataFile.print(x, 2); dataFile.print(" ");
      dataFile.print(y, 2); dataFile.print(" ");
      dataFile.println(z, 2);
    }
  } else {
    Serial.println("Sensor error");
  }

  // Rotate table 5°
  for (int i = 0; i < STEPS_PER_STEP; i++) {
    digitalWrite(STEP_PIN_1, HIGH);
    delayMicroseconds(STEP_DELAY_US);
    digitalWrite(STEP_PIN_1, LOW);
    delayMicroseconds(STEP_DELAY_US);
  }

  rotationAngle += DEGREE_STEP;
  delay(500); // Complete 0.5 second per step

  // After full rotation
  if (rotationAngle >= 360) {
    // Move sensor up 1 mm
    for (int i = 0; i < (int)STEPS_PER_MM; i++) {
      digitalWrite(STEP_PIN_2, HIGH);
      delayMicroseconds(STEP_DELAY_US);
      digitalWrite(STEP_PIN_2, LOW);
      delayMicroseconds(STEP_DELAY_US);
    }

    currentZ += 1.0;       // Increase height by 1 mm
    rotationAngle = 0;     // Reset rotation
  }
}
