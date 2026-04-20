#pragma once
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
#include "Data.h"
#include "Constants.h"
#include "Pins.h"

bool initGPS();
void updateGPS();