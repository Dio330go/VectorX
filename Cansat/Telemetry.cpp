#include <SPI.h>
#include <RadioLib.h>
#include "Telemetry.h"
#include "Constants.h"

SX1278 radio = new Module(LORA_NSS, LORA_DIO0, RADIOLIB_NC, RADIOLIB_NC);

volatile bool transmittedFlag = false;
int transmissionState = RADIOLIB_ERR_NONE;
int count = 0;

const float LAUNCH_LAT = 39.2087835;
const float LAUNCH_LON = -8.0558733;
const int scale = 10000;

float gpslon(float lon) {
    return (lon - LAUNCH_LON);
}
float gpslat(float lat) {
    return (lat - LAUNCH_LAT);
}

void packTelemetry(uint8_t* packet, int ms) {
  uint16_t pressure = (uint16_t)((SEALEVELPRESSURE_HPA - sensors.pressure) * 100 + 0.5);
  uint8_t temp       = (uint8_t)(sensors.temperature * 5 + 0.5);

  int16_t ax = (int16_t)(sensors.accel1_x * 100);
  int16_t ay = (int16_t)(sensors.accel1_y * 100);
  int16_t az = (int16_t)(sensors.accel1_z * 100);

  int8_t angX = (int8_t)(sensors.roll1);
  int8_t angY = (int8_t)(sensors.pitch1);
  int8_t angZ = (int8_t)(sensors.yaw1);

  int16_t gpsX = (int16_t)(gpslon(sensors.gps_lng/100) * scale);
  int16_t gpsY = (int16_t)(gpslat(sensors.gps_lat/100) * scale);

  int16_t mili = (int16_t)(ms);

  packet[0] = (pressure >> 8) & 0xFF;
  packet[1] = pressure & 0xFF;

  packet[2] = (int8_t)temp;

  packet[3] = (ax >> 8) & 0xFF;
  packet[4] = ax & 0xFF;

  packet[5] = (ay >> 8) & 0xFF;
  packet[6] = ay & 0xFF;

  packet[7] = (az >> 8) & 0xFF;
  packet[8] = az & 0xFF;

  packet[9]  = (uint8_t)angX;
  packet[10] = (uint8_t)angY;
  packet[11] = (uint8_t)angZ;

  packet[12] = (gpsX >> 8) & 0xFF;
  packet[13] = gpsX & 0xFF;

  packet[14] = (gpsY >> 8) & 0xFF;
  packet[15] = gpsY & 0xFF;

  packet[16] = (mili >> 8) & 0xFF;
  packet[17] = mili & 0xFF;
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
  int state = radio.begin();
  if(state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    return false;
  }

  radio.setFrequency(434.450);
  radio.setSpreadingFactor(7);
  radio.setCodingRate(5);
  radio.setPacketSentAction(setFlag);

  Serial.print(F("[SX1278] Sending first packet ... "));
  uint8_t packet[18] = {};
  transmissionState = radio.startTransmit(packet, 18);
  status.lora_ok = true;
  return true;
}

void sendTelemetry(int ms) {
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

  uint8_t packet[18];
  packTelemetry(packet, ms);

  Serial.println(F("[SX1278] Sending another packet ... "));
  transmissionState = radio.transmit(packet, 18);
}