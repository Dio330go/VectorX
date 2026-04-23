#include "Sensors.h"

uint32_t lastLSM = 0;
uint32_t lastICM = 0;
uint32_t lastLoRa = 0;
uint32_t lastBMP = 0;
uint32_t lastSD = 0;

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
  if (status.gps_ok) updateGPS();

  uint32_t now = micros();

  if (status.bmp_ok && now - lastBMP >= 200000) {  // example: 200 ms
    lastBMP = now;
    updateBMP();
  }

  if (status.icm_ok && now - lastICM >= 10000) {   // 10 ms
    lastICM = now;
    updateICM();
  }

  if (status.lsm_ok && now - lastLSM >= 10000) {   // 10 ms
    lastLSM = now;
    updateLSM();
  }

  if (status.sd_ok && now - lastSD >= 100000) {  // example: 100 ms
    lastSD = now;
    logData();
  }

  if (status.lora_ok && now - lastLoRa >= 100000) { // example: 100 ms
    lastLoRa = now;
    sendTelemetry();
  }
}