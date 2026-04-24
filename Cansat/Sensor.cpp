#include "Sensors.h"

int buzzer = 26;

uint32_t lastLSM = 0;
uint32_t lastICM = 0;
uint32_t lastLoRa = 0;
uint32_t lastBMP = 0;
uint32_t lastSD = 0;
uint32_t lastbuzzer = 0; 

bool initSensors() {
  bool ok = true;

  if (!initBMP()) ok = false;
  if (!initLogger()) ok = false;
  if (!initLSM()) ok = false;
  if (!initICM()) ok = false;
  // if (!initGPS()) ok = false;
  if (!initTelemetry()) ok = false;
  pinMode(buzzer, OUTPUT);

  return ok;
}

void updateSensors() {
  // if (status.gps_ok) updateGPS();

  uint32_t now = millis();

  // Serial.println(sensors.gps_lat);
  // Serial.println(sensors.gps_lng);
  // Serial.println();

  if (status.bmp_ok && now - lastBMP >= 200) {  // example: 200 ms
    lastBMP = now;
    updateBMP();
  }

  if (status.icm_ok && now - lastICM >= 10) {   // 10 ms
    lastICM = now;
    updateICM();
  }

  if (status.lsm_ok && now - lastLSM >= 10) {   // 10 ms
    lastLSM = now;
    updateLSM();
  }

  if (status.sd_ok && now - lastSD >= 100) {  // example: 100 ms
    logData(now - lastSD);
    Serial.println(now - lastSD);
    lastSD = now;
  }

  if (status.lora_ok && now - lastLoRa >= 250) { // example: 250 ms
    sendTelemetry(now - lastLoRa);
    lastLoRa = now;
  }
  // if (now - lastbuzzer >= 2000){
  //   digitalWrite(buzzer, LOW);
  // }
  // if (now - lastbuzzer >= 4000){
  //   digitalWrite(buzzer, HIGH);
  //   lastbuzzer = 0;
  // }
}