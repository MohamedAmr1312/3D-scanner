#include <Wire.h>
#include <VL6180X.h>
#include <SD.h>
#include <SPI.h>
#include <AccelStepper.h>

// --- Pin Definitions ---
#define STEP_PIN_1 9  // Rotation stepper
#define DIR_PIN_1 8
#define STEP_PIN_2 7  // Z-axis stepper
#define DIR_PIN_2 6
#define SD_CS_PIN 10  // SD card chip select

// --- Stepper Motor & Scan Parameters ---
#define STEPS_PER_REV 200           // 1.8° per step
#define DEGREES_PER_STEP 1.8
#define MICROSTEPS 1                // Set according to driver
#define LEAD_SCREW_PITCH 2.0        // mm/rev
#define STEPS_PER_MM (STEPS_PER_REV / LEAD_SCREW_PITCH)

#define ROTATION_SPEED 50          // Steps per second
#define ROTATION_ACCEL 50           // Steps per second^2

#define MAX_VALID_DISTANCE 200      // mm
#define Z_INCREMENT 2.0             // mm per scan layer
#define MAX_HEIGHT 500.0            // Total scan height in mm

// --- Objects ---
VL6180X sensor;
AccelStepper rotStepper(AccelStepper::DRIVER, STEP_PIN_1, DIR_PIN_1);
File dataFile;

// --- Variables ---
float currentHeight = 0;
int rotationStep = 0;
const int STEPS_PER_ROTATION = 200;  // Full 360° = 200 steps

void setup() {
  // Z-axis pins
  pinMode(STEP_PIN_2, OUTPUT);
  pinMode(DIR_PIN_2, OUTPUT);
  digitalWrite(DIR_PIN_2, HIGH); // Upward

  // Serial and sensor init
  Serial.begin(9600);
  Wire.begin();
  sensor.init();
  sensor.setScaling(3);
  sensor.configureDefault();
  sensor.setTimeout(100);
  Serial.println("Sensor initialized");

  // SD card init
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("SD card init failed!");
    //while (1); // Halt
  }

  dataFile = SD.open("scan.xyz", FILE_WRITE);
  if (!dataFile) {
    Serial.println("Error opening file!");
  }

  // Stepper init
  rotStepper.setMaxSpeed(ROTATION_SPEED);
  rotStepper.setAcceleration(ROTATION_ACCEL);
  rotStepper.setCurrentPosition(0);
}

void loop() {
  // Stop scanning if maximum height reached
  if (currentHeight >= MAX_HEIGHT) {
    Serial.println("Scan complete.");
    if (dataFile) dataFile.close();
    while (1); // Halt
  }

  // Rotate and scan
  if (rotationStep < STEPS_PER_ROTATION) {
    if (!rotStepper.isRunning()) {
      // Take distance reading
      float distance = sensor.readRangeSingleMillimeters();

      if (sensor.timeoutOccurred()) {
        Serial.println("Sensor timeout!");
      } 
      else if (distance >= MAX_VALID_DISTANCE) {
        Serial.println("No object detected!");
      } 
      else {
        // Polar to Cartesian conversion
        float angle = rotationStep * DEGREES_PER_STEP * (PI / 180.0);
        float x = (distance-62.5) * cos(angle);
        float y = currentHeight + 0.05;
        float z = (distance-62.5) * sin(angle);

        // Output
        Serial.print(x, 2); Serial.print(" ");
        Serial.print(y, 2); Serial.print(" ");
        Serial.println(z, 2);

        if (dataFile) {
          dataFile.print(x, 2); dataFile.print(" ");
          dataFile.print(y, 2); dataFile.print(" ");
          dataFile.println(z, 2);
        }
      }

      // Step motor by 1
      rotStepper.move(1);
      rotationStep++;
    }

    rotStepper.run(); // Must be called often
  } 
  else {
    // Finished one full revolution — move up Z
    moveZAxis(Z_INCREMENT);
    currentHeight += Z_INCREMENT;
    rotationStep = 0;

    if (dataFile) {
      dataFile.flush();
    }

    delay(500); // Optional small pause for stability
  }
}

void moveZAxis(float mm) {
  long steps = mm * STEPS_PER_MM;
  digitalWrite(DIR_PIN_2, HIGH); // Upward direction

  for (long i = 0; i < steps; i++) {
    digitalWrite(STEP_PIN_2, HIGH);
    delayMicroseconds(850);
    digitalWrite(STEP_PIN_2, LOW);
    delayMicroseconds(850);
  }
}
