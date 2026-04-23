#pragma once
#include <SPI.h>
#include <SD.h>
#include "Pins.h"
#include "Data.h"

bool initLogger();
void logData(int ms);