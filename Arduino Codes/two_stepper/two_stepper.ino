// Define stepper motor pins
#define STEP_PIN_1 7
#define DIR_PIN_1 6
#define STEP_PIN_2 9
#define DIR_PIN_2 8

// Stepper motor parameters
#define STEPS_PER_REV 200      // Steps per revolution
#define STEP_ANGLE 10          // Each move is 10 degrees
#define STEPS_PER_STEP (STEPS_PER_REV / 360.0 * STEP_ANGLE)

// Lead screw parameters
#define LEAD_SCREW_PITCH 2     // mm per revolution
#define STEPS_PER_MM (STEPS_PER_REV / LEAD_SCREW_PITCH)

// Step timing
#define STEP_DELAY 800         // Microseconds between steps (adjust for speed)

void setup() {
  pinMode(STEP_PIN_1, OUTPUT);
  pinMode(DIR_PIN_1, OUTPUT);
  pinMode(STEP_PIN_2, OUTPUT);
  pinMode(DIR_PIN_2, OUTPUT);

  digitalWrite(DIR_PIN_1, HIGH); // Set direction for stepper1
  digitalWrite(DIR_PIN_2, HIGH); // Set direction for stepper2
}

void loop() {
  static int rotation_count = 0;

  // Step stepper1 for 10 degrees
  for (int i = 0; i < STEPS_PER_STEP; i++) {
    digitalWrite(STEP_PIN_1, HIGH);
    delayMicroseconds(1500);
    digitalWrite(STEP_PIN_1, LOW);
    delayMicroseconds(1500);
  }

  rotation_count += STEP_ANGLE;
  delay(500); // Wait for 1 seconds

  // If completed 360 degrees, move stepper2 by 2 mm
  if (rotation_count >= 360) {
    for (int i = 0; i < (int)(STEPS_PER_MM * 2); i++) {
      digitalWrite(STEP_PIN_2, HIGH);
      delayMicroseconds(700);
      digitalWrite(STEP_PIN_2, LOW);
      delayMicroseconds(700);
    }
    rotation_count = 0;
  }
}
