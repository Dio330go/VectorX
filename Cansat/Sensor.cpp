#include "Sensors.h"

bool initSensors() {
  bool ok = true;

  if (!initBMP()) ok = false;
  if (!initLogger()) ok = false;
  if (!initLSM()) ok = false;
  if (!initICM()) ok = false;
  if (!initGPS()) ok = false;
  if (!initTelemetry()) ok = false;

  return ok;
}

void updateSensors() {
  for(int i = 0; i < 3; i++)
  {
    updateBMP();
    for(int i = 0; i < 100; i++)
    {
      updateLSM();
      // updateICM();
      delay(10);
      for(int i = 0; i < 10; i++)
      {
        updateGPS();
        delay(1);
      }
    }
  }
  // logData();
  sendTelemetry();
}