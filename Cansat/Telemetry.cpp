#include <SPI.h>
#include <RadioLib.h>
#include "Telemetry.h"

SX1278 radio = new Module(LORA_NSS, LORA_DIO0, RADIOLIB_NC, RADIOLIB_NC);

volatile bool transmittedFlag = false;
int transmissionState = RADIOLIB_ERR_NONE;
int count = 0;

const float LAUNCH_LAT = 38.7169;
const float LAUNCH_LON = -9.1399;

float gpsToOffsetX(float lon) {
    return (lon - LAUNCH_LON) * cos(LAUNCH_LAT * PI / 180.0) * 6371000.0;
}
float gpsToOffsetY(float lat) {
    return (lat - LAUNCH_LAT) * 6371000.0 * (PI / 180.0);
}

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
  radio.setFrequency(434.450);
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
  TelemetryPacket pkt = {};
  transmissionState = radio.startTransmit((uint8_t*)&pkt, sizeof(pkt));
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

  TelemetryPacket pkt;
  pkt.pressure    = (uint16_t)(sensors.pressure * 100);
  pkt.temperature = (int8_t)  (sensors.temperature  * 2);
  pkt.ax          = (int16_t) (sensors.accel1_x    * 100);
  pkt.ay          = (int16_t) (sensors.accel1_y    * 100);
  pkt.az          = (int16_t) (sensors.accel1_z    * 100);
  pkt.angX        = (int8_t)  (sensors.roll1);
  pkt.angY        = (int8_t)  (sensors.pitch1);
  pkt.angZ        = (int8_t)  (sensors.yaw1);
  pkt.gpsX        = (int16_t) (gpsToOffsetX(sensors.gps_lng) * 10);
  pkt.gpsY        = (int16_t) (gpsToOffsetY(sensors.gps_lat) * 10);

  Serial.println(F("[SX1278] Sending another packet ... "));
  transmissionState = radio.transmit((uint8_t*)&pkt, sizeof(pkt));
}