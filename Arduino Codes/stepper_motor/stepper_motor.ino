#include <AccelStepper.h>

// Define stepper motor connections and interface type
#define STEP_PIN 7
#define DIR_PIN 6
#define MOTOR_INTERFACE_TYPE 1

// Create an instance of AccelStepper
AccelStepper stepper(MOTOR_INTERFACE_TYPE, STEP_PIN, DIR_PIN);

// Define step size in steps (assuming 200 steps per revolution)
#define STEPS_PER_REV 200  // Adjust based on your motor
#define STEP_ANGLE 10      // Degrees per step
#define STEPS_PER_STEP (STEPS_PER_REV / 360.0 * STEP_ANGLE)

void setup() {
    stepper.setMaxSpeed(1000); // Set max speed
    stepper.setAcceleration(500); // Set acceleration
}

void loop() {
    stepper.move(STEPS_PER_STEP); // Move stepper 10 degrees
    stepper.runToPosition(); // Wait until motion is complete
    delay(2000); // Pause for 2 seconds
}
