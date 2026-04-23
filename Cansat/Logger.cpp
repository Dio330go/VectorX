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

  SD.remove("/data.csv");

  File file = SD.open("/data.csv", FILE_WRITE);

  if (file) {
    file.println("pressure,temperature,altitude,accel1_x,accel1_y,accel1_z,roll1,pitch1,yaw1,accel2_x,accel2_y,accel2_z,roll2,pitch2,yaw2,gps_lat,gps_lng,gps_fix");
    file.close();
  }

  status.sd_ok = true;
  return true;
}

void logData(int ms) {
  File data = SD.open("/data.csv", FILE_WRITE);

  if (data) {
    data.print(sensors.pressure); data.print(",");
    data.print(sensors.temperature); data.print(",");
    data.print(sensors.altitude); data.print(",");

    data.print(sensors.accel1_x); data.print(",");
    data.print(sensors.accel1_y); data.print(",");
    data.print(sensors.accel1_z); data.print(",");

    data.print(sensors.roll1); data.print(",");
    data.print(sensors.pitch1); data.print(",");
    data.print(sensors.yaw1); data.print(",");

    data.print(sensors.accel2_x); data.print(",");
    data.print(sensors.accel2_y); data.print(",");
    data.print(sensors.accel2_z); data.print(",");

    data.print(sensors.roll2); data.print(",");
    data.print(sensors.pitch2); data.print(",");
    data.print(sensors.yaw2); data.print(",");

    data.print(sensors.gps_lat, 6); data.print(",");
    data.print(sensors.gps_lng, 6); data.print(",");

    data.print(ms); data.print(",");

    data.println(sensors.gps_fix ? 1 : 0); // bool as 0/1
    data.close();
  }
}