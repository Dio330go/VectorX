#include "ICM.h"

// Madgwick filter;

Adafruit_ICM20948 icm;
// uint16_t measurement_delay_us = 65535; // Delay between measurements for testing

// float mag_min[3] = {1000, 1000, 1000};
// float mag_max[3] = {-1000, -1000, -1000};

bool initICM() {
  Wire1.setSDA(ICM_SDA);
  Wire1.setSCL(ICM_SCL);
  Wire1.begin();

  if (!icm.begin_I2C(ICM20948_I2CADDR_DEFAULT, &Wire1)) {
    status.icm_ok = false;
    return false;
  }

  icm.setAccelRange(ICM20948_ACCEL_RANGE_16_G); // 2G 4G 8G 16G
  icm.setGyroRange(ICM20948_GYRO_RANGE_2000_DPS); // 250DPS 500DPS 1000DPS 2000DPS
  icm.setMagDataRate(AK09916_MAG_DATARATE_10_HZ); // SINGLE 10Hz 20Hz 50Hz 100Hz
  icm.setAccelRateDivisor(4095);
  icm.setGyroRateDivisor(255);

  // uint16_t accel_divisor = icm.getAccelRateDivisor(); 
  // float accel_rate = 1125 / (1.0 + accel_divisor); // Hz
  // uint8_t gyro_divisor = icm.getGyroRateDivisor();
  // float gyro_rate = 1100 / (1.0 + gyro_divisor); // Hz

  status.icm_ok = true;
  return true;

}

void updateICM() {
  //  /* Get a new normalized sensor event */
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t mag;
  sensors_event_t temp;

  if (!icm.getEvent(&accel, &gyro, &temp, &mag)) {
    status.lsm_ok = false;
    return;
  }

  // ---- accelerometer (m/s² → g) ----
  sensors.accel2_x = accel.acceleration.x;
  sensors.accel2_y = accel.acceleration.y;
  sensors.accel2_z = accel.acceleration.z;

  // ---- gyroscope (rad/s, Adafruit already gives rad/s) ----
  sensors.gyro2_x = gyro.gyro.x;
  sensors.gyro2_y = gyro.gyro.y;
  sensors.gyro2_z = gyro.gyro.z;

  // ---- magnetometer — apply hard+soft iron correction ----
  sensors.mag2_x = mag.magnetic.x;
  sensors.mag2_y = mag.magnetic.y;
  sensors.mag2_z = mag.magnetic.z;
}