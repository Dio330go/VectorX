#include "Data.h"
#include "BMP.h"
#include "Constants.h"
// #include "State.h"
// #include "Mission.h"
// #include "Sensors.h"
// #include "Telemetry.h"
// #include "Logger.h"
// #include "Buzzer.h"

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

  if (!initBMP()) {
    Serial.println("BMP failed!");
    while (1){};
  }
}

void loop() {
  updateBMP();

  Serial.print("Altitude: ");
  Serial.println(sensors.altitude);

  delay(3000);
}