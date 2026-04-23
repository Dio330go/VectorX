#pragma once
#include <RadioLib.h>
#include "Pins.h"
#include "Data.h"
#pragma pack(1)
struct TelemetryPacket {
    uint16_t pressure;         // kPa × 100
    int8_t   temperature;      // °C × 2
    int16_t  ax, ay, az;       // m/s² × 100
    int8_t   angX, angY, angZ; // graus inteiros
    int16_t  gpsX, gpsY;       // metros × 10
};
#pragma pack()

bool initTelemetry();
void sendTelemetry();