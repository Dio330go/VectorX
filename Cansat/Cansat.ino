#include "Data.h"
#include "Sensors.h"
#include "Constants.h"
// #include "State.h"
// #include "Mission.h"
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


  if (!initSensors()) {
    Serial.println("Something failed!");
    while (1){};
  }
  Serial.println("Initialized");

}

void loop() {
  updateSensors();

  // Serial.print("Altitude: ");
  // Serial.println(sensors.altitude);

  delay(20);
}