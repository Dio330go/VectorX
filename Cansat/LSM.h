#pragma once
#include <Wire.h>
#include <Adafruit_LSM6DS3TRC.h>
#include <Adafruit_LIS3MDL.h>
#include <Adafruit_Sensor.h>
#include "Pins.h"
#include "Data.h"

// #include <MadgwickAHRS.h>

extern Adafruit_LSM6DS3TRC lsm6ds;
extern Adafruit_LIS3MDL lis3mdl;

bool initLSM();
void updateLSM();

// Madgwick filter;