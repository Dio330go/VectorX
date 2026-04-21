#include "Logger.h"

bool initLogger() {
  // Initialize SPI manually (important for Pico)

  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS, HIGH);

  SPI1.setSCK(SD_SCK);
  SPI1.setTX(SD_TX);
  SPI1.setRX(SD_RX);

  if (!SD.begin(SD_CS, SPI1)) {
    status.sd_ok = false;
    return false;
  }

  SD.remove("/acelerometro.csv");

  File file = SD.open("/acelerometro.csv", FILE_WRITE);

  if (file) {
    file.println("ax,ay,az,gx,gy,gz,mx,my,mz");
    file.close();
  }

  status.sd_ok = true;
  return true;
}

void logData() {
  File file = SD.open("/acelerometro.csv", FILE_WRITE);

  if (file) {


    file.close();
  }
}