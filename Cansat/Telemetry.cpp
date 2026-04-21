#include <SPI.h>
#include <RadioLib.h>
#include "Telemetry.h"

SX1278 radio = new Module(LORA_NSS, LORA_DIO0, RADIOLIB_NC, RADIOLIB_NC);

volatile bool transmittedFlag = false;
int transmissionState = RADIOLIB_ERR_NONE;
int count = 0;

void setFlag() {
  transmittedFlag = true;
}

bool initTelemetry() {
  Serial.begin(9600);

  SPI.setSCK(LORA_SCK);
  SPI.setRX(LORA_MISO);
  SPI.setTX(LORA_MOSI);
  SPI.begin();

  Serial.print(F("[SX1278] Initializing ... "));
  int state = radio.begin();
  if(state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    return false;
  }

  radio.setPacketSentAction(setFlag);

  Serial.print(F("[SX1278] Sending first packet ... "));
  transmissionState = radio.startTransmit("Hello World!");
  return true;
}

void sendTelemetry() {
  if(!transmittedFlag) {
    return;
  }

  transmittedFlag = false;

  if(transmissionState == RADIOLIB_ERR_NONE) {
    Serial.println(F("transmission finished!"));
    Serial.println();
  } else {
    Serial.print(F("failed, code "));
    Serial.println(transmissionState);
  }

  radio.finishTransmit();

  String msg = String(sensors.accel1_x) + ',' + String(sensors.accel1_y) + ',' + String(sensors.accel1_z) + ',' + String(sensors.roll1) + ',' + String(sensors.pitch1) + ',' + String(sensors.yaw1);
  Serial.println(F("[SX1278] Sending another packet ... "));
  transmissionState = radio.startTransmit(msg);
}