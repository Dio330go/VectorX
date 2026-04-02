#pragma once
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP3XX.h>
#include "Pins.h"
#include "Data.h"
#include "Constants.h"

extern Adafruit_BMP3XX bmp;

bool initBMP();
void updateBMP();