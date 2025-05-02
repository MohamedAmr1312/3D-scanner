#include <Wire.h>
#include "Adafruit_VL6180X.h"

// Stepper pins
#define DIR_PIN_1 2
#define STEP_PIN_1 3
#define DIR_PIN_2 4
#define STEP_PIN_2 5

// Stepper config
const float STEP_ANGLE = 1.8;                // degrees
const int STEP_DEGREES = 5;                  // rotate 5° per step
const int STEPS_PER_STEP = STEP_DEGREES / STEP_ANGLE;

const float LEAD_SCREW_PITCH = 2.0;          // mm/rev
const int STEPS_PER_REV = 200;               // full steps
const int STEPS_PER_MM = STEPS_PER_REV / LEAD_SCREW_PITCH;

Adafruit_VL6180X sensor = Adafruit_VL6180X();

int rotationCount = 0;
float currentHeight = 0.0;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  if (!sensor.begin()) {
    Serial.println("Failed to find VL6180X sensor!");
    while (1);
  }

  pinMode(DIR_PIN_1, OUTPUT);
  pinMode(STEP_PIN_1, OUTPUT);
  pinMode(DIR_PIN_2, OUTPUT);
  pinMode(STEP_PIN_2, OUTPUT);

  digitalWrite(DIR_PIN_1, HIGH); // clockwise
  digitalWrite(DIR_PIN_2, HIGH); // upward
}

void loop() {
  // Rotate stepper 1 by 5 degrees
  for (int i = 0; i < STEPS_PER_STEP; i++) {
    digitalWrite(STEP_PIN_1, HIGH);
    delayMicroseconds(1500);
    digitalWrite(STEP_PIN_1, LOW);
    delayMicroseconds(1500);
  }

  // Read distance from VL6180X (returns mm)
  uint8_t distance = sensor.readRange();
  uint8_t status = sensor.readRangeStatus();

  if (status == VL6180X_ERROR_NONE) {
    float angleRad = radians(rotationCount);
    float x = distance * cos(angleRad);
    float y = distance * sin(angleRad);
    float z = currentHeight;

    Serial.print(x, 2); Serial.print(" "); 
    Serial.print(y, 2); Serial.print(" "); 
    Serial.print(z, 2); Serial.print(" "); 
    Serial.println(" mm");
  } else {
    Serial.print("Sensor error code: ");
    Serial.println(status);
  }

  rotationCount += STEP_DEGREES;
  delay(500); // wait 0.5 sec

  // Move up after full 360°
  if (rotationCount >= 360) {
    for (int i = 0; i < STEPS_PER_MM; i++) {
      digitalWrite(STEP_PIN_2, HIGH);
      delayMicroseconds(700);
      digitalWrite(STEP_PIN_2, LOW);
      delayMicroseconds(700);
    }
    currentHeight += 1.0;
    rotationCount = 0;
  }
}
