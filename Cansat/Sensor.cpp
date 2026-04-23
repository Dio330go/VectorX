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
    if(status.bmp_ok) updateBMP();
    for(int i = 0; i < 100; i++)
    {
      if(status.lsm_ok) updateLSM();
      if(status.icm_ok) updateICM();z
      delay(10);
      for(int i = 0; i < 10; i++)
      {
        if(status.gps_ok) updateGPS();
        delay(1);
      }
    }
  }
  if(status.lora_ok) logData();
  if(status.lora_ok) sendTelemetry();
}