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
  // Serial.println("ax,ay,az,gx,gy,gz,mx,my,mz");
  // Serial.print(sensors.accel1_x);
  // Serial.print(",");  
  // Serial.print(sensors.accel1_y);
  // Serial.print(",");
  // Serial.print(sensors.accel1_z);
  // Serial.print(",");

  // Serial.print(sensors.gyro1_x);
  // Serial.print(",");
  // Serial.print(sensors.gyro1_y);
  // Serial.print(",");
  // Serial.print(sensors.gyro1_z);
  // Serial.print(",");

  // Serial.print(sensors.mag1_x);
  // Serial.print(",");
  // Serial.print(sensors.mag1_y);
  // Serial.print(",");
  // Serial.println(sensors.mag1_z);
  updateSensors();

  delay(3000);
}