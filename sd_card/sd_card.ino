#include <SPI.h>
#include <SD.h>

#define SD_CS 17  // Chip Select pin

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("Initializing SD card...");

  // Initialize SPI manually (important for Pico)
  SPI.setSCK(18);
  SPI.setTX(19);
  SPI.setRX(16);

  if (!SD.begin(SD_CS)) {
    Serial.println("SD card initialization failed!");
    return;
  }

  Serial.println("SD card initialized.");

  // --- WRITE FILE ---
  File file = SD.open("/test.txt", FILE_WRITE);

  if (file) {
    file.println("Hello from Raspberry Pi Pico (Arduino)!");
    file.close();
    Serial.println("File written.");
  } else {
    Serial.println("Error opening file for writing.");
  }

  // --- READ FILE ---
  file = SD.open("/test.txt");

  if (file) {
    Serial.println("Reading file:");

    while (file.available()) {
      Serial.write(file.read());
    }

    file.close();
  } else {
    Serial.println("Error opening file for reading.");
  }
}

void loop() {
  // nothing here
}