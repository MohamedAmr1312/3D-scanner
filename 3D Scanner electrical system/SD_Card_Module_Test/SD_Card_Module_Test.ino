#include <SPI.h>
#include <SD.h>

#define SD_CS 10  // Chip Select pin (change if needed)

void setup() {
    Serial.begin(115200);
    Serial.print("Initializing SD card...");

    if (!SD.begin(SD_CS)) {
        Serial.println("Card failed or not present.");
        return;
    }
    
    Serial.println("SD card initialized successfully!");
}

void loop() {
    
