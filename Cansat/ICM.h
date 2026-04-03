#pragma once
#include <Adafruit_ICM20X.h>
#include <Adafruit_ICM20948.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include "Pins.h"
#include "Data.h"

// #include <MadgwickAHRS.h>

extern Adafruit_ICM20948 icm;

bool initICM();
void updateICM();

// Madgwick filter;