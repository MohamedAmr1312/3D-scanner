#include <Wire.h>
#include "Adafruit_VL6180X.h"

Adafruit_VL6180X sensor = Adafruit_VL6180X();

void setup() {
    Serial.begin(115200);
    sensor.begin();
}

void loop() {
    Serial.println(sensor.readRange());
    delay(500);
}
