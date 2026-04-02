#include "Data.h"
#include "Sensors.h"
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

  if (!initSensors()) {
    Serial.println("Something failed!");
    while (1){};
  }

  Serial.println("ax,ay,az,gx,gy,gz,mx,my,mz");
}

void loop() {
  updateSensors();

  // Serial.print("Altitude: ");
  // Serial.println(sensors.altitude);

  // Serial.print("Altitude: ");
  // Serial.println(sensors.altitude);

  Serial.print(sensors.accel1_x);
  Serial.print(",");  
  Serial.print(sensors.accel1_y);
  Serial.print(",");
  Serial.print(sensors.accel1_z);
  Serial.print(",");

  Serial.print(sensors.gyro1_x);
  Serial.print(",");
  Serial.print(sensors.gyro1_y);
  Serial.print(",");
  Serial.print(sensors.gyro1_z);
  Serial.print(",");

  Serial.print(sensors.mag1_x);
  Serial.print(",");
  Serial.print(sensors.mag1_y);
  Serial.print(",");
  Serial.println(sensors.mag1_z);

  delay(3000);
}