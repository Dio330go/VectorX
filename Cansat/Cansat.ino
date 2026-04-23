#include "Data.h"
#include "Sensors.h"
#include "Constants.h"
// #include "Mission.h"

SensorData sensors;
SystemStatus status;
MissionData mission;

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(LED_BUILTIN, LOW);
  delay(STARTING_DELAY);
  digitalWrite(LED_BUILTIN, HIGH); // reached setup start

  unsigned long start = millis();
  while (!Serial && millis() - start < 3000) {}

  Serial.println("Initialized");

  if (!initSensors()) {
    if (!status.bmp_ok) Serial.println("BMP Wrong");
    if (!status.sd_ok) Serial.println("SD Wrong");
    if (!status.lsm_ok) Serial.println("LSM Wrong");
    if (!status.icm_ok) Serial.println("ICM Wrong");
    if (!status.gps_ok) Serial.println("GPS Wrong");
    if (!status.lora_ok) Serial.println("Lora Wrong");
  }
}

void loop() {
  updateSensors();
}