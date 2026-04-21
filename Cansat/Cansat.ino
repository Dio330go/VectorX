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
    Serial.println("Something failed!");
    while (1){};
  }
}

void loop() {
  // Serial.print("Ax = ");
  // Serial.print(sensors.accel1_x);
  // Serial.print("    Ay = ");
  // Serial.print(sensors.accel1_y);
  // Serial.print("    Az = ");
  // Serial.println(sensors.accel1_z);

  // Serial.print("roll = ");
  // Serial.print(sensors.roll1);
  // Serial.print("    pitch = ");
  // Serial.print(sensors.pitch1);
  // Serial.print("    yaw = ");
  // Serial.println(sensors.yaw1);
  // Serial.println();

  updateSensors();
}