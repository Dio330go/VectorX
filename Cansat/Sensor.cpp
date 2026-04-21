#include "Sensors.h"

bool initSensors() {
  bool ok = true;

  // if (!initBMP()) ok = false;
  // if (!initLogger()) ok = false;
  // if (!initLSM()) ok = false;
  // if (!initICM()) ok = false;
  if (!initGPS()) ok = false;
  // if (!initTelemetry()) ok = false;

  return ok;
}

void updateSensors() {
  // Serial.println("Sensors");
  // updateBMP();
  // for(int i = 0; i < 100; i++)
  // {
  //   updateLSM();
  //   delay(10);
  // }
  // updateICM();
  // logData();
  updateGPS();
  // sendTelemetry();
}