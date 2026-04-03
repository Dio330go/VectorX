#include "Sensors.h"

// #include "GPS.h"
// #include "IMU.h"

bool initSensors() {
  bool ok = true;

  // if (!initBMP()) {
  //   ok = false;
  // }
  if (!initLogger()) ok = false;
  if (!initLSM()) ok = false;

  return ok;
}

void updateSensors() {
  // updateBMP();

  updateLSM();
  logData();
}