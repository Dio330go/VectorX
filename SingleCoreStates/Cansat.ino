#include "Data.h"
#include "State.h"
#include "Mission.h"
#include "Sensors.h"
#include "Telemetry.h"
#include "Logger.h"
#include "Buzzer.h"

void setup() {
  setupMission();
}

void loop() {
  updateSensors();
  runMissionState();
  updateTelemetry();
  updateLogger();
  updateBuzzer();
}