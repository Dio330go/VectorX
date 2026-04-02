#include "LSM.h"

Adafruit_LSM6DS3TRC lsm6ds;
Adafruit_LIS3MDL lis3mdl;
// Madgwick            filter;

// magnetometer hard-iron offsets + soft-iron scale (set by calibrateMag)
// float mx_offset = 0, my_offset = 0, mz_offset = 0;
// float mx_scale  = 1, my_scale  = 1, mz_scale  = 1;

// unsigned long lastUpdate = 0;
// const int interval = 5000; // 200 Hz

// ---------------------------------------------------------------- calibration

// void calibrateMag() {
//   Serial.println("==============================================");
//   Serial.println("Mag calibration — rotate sensor in a slow");
//   Serial.println("figure-8 covering ALL orientations for 20s.");
//   Serial.println("Starting in 3 seconds...");
//   Serial.println("==============================================");
//   delay(3000);

//   float mx_min =  1e9, my_min =  1e9, mz_min =  1e9;
//   float mx_max = -1e9, my_max = -1e9, mz_max = -1e9;

//   unsigned long start    = millis();
//   unsigned long duration = 20000;
//   unsigned long lastPrint = 0;

//   while (millis() - start < duration) {
//     sensors_event_t event;
//     lis3mdl.getEvent(&event);

//     float mx = event.magnetic.x;
//     float my = event.magnetic.y;
//     float mz = event.magnetic.z;

//     if (mx < mx_min) mx_min = mx;  if (mx > mx_max) mx_max = mx;
//     if (my < my_min) my_min = my;  if (my > my_max) my_max = my;
//     if (mz < mz_min) mz_min = mz;  if (mz > mz_max) mz_max = mz;

//     if (millis() - lastPrint > 1000) {
//       Serial.print("Time left: ");
//       Serial.print((duration - (millis() - start)) / 1000);
//       Serial.println("s");
//       lastPrint = millis();
//     }
//     delay(10);
//   }

//   mx_offset = (mx_max + mx_min) / 2.0;
//   my_offset = (my_max + my_min) / 2.0;
//   mz_offset = (mz_max + mz_min) / 2.0;

//   float mx_range  = (mx_max - mx_min) / 2.0;
//   float my_range  = (my_max - my_min) / 2.0;
//   float mz_range  = (mz_max - mz_min) / 2.0;
//   float avg_range = (mx_range + my_range + mz_range) / 3.0;
//   mx_scale = avg_range / mx_range;
//   my_scale = avg_range / my_range;
//   mz_scale = avg_range / mz_range;

//   Serial.println("--- Mag calibration results ---");
//   Serial.print("Offsets  X: "); Serial.print(mx_offset);
//   Serial.print("  Y: ");        Serial.print(my_offset);
//   Serial.print("  Z: ");        Serial.println(mz_offset);
//   Serial.print("Scales   X: "); Serial.print(mx_scale);
//   Serial.print("  Y: ");        Serial.print(my_scale);
//   Serial.print("  Z: ");        Serial.println(mz_scale);
//   Serial.println("Mag calibration done.");
// }

// ----------------------------------------------------------------------- setup

bool initLSM() {
  Wire.setSDA(LSM_SDA);
  Wire.setSCL(LSM_SCL);
  Wire.begin();

  if (!lsm6ds.begin_I2C()) {
    status.lsm_ok = false;
    return false;
  }
  if (!lis3mdl.begin_I2C()) {
    status.lsm_ok = false;
    return false;
  }

  // configure LSM6DS3
  lsm6ds.setAccelRange(LSM6DS_ACCEL_RANGE_2_G);
  lsm6ds.setGyroRange(LSM6DS_GYRO_RANGE_250_DPS);
  lsm6ds.setAccelDataRate(LSM6DS_RATE_104_HZ);
  lsm6ds.setGyroDataRate(LSM6DS_RATE_104_HZ);

  // configure LIS3MDL
  lis3mdl.setPerformanceMode(LIS3MDL_ULTRAHIGHMODE);
  lis3mdl.setOperationMode(LIS3MDL_CONTINUOUSMODE);
  lis3mdl.setDataRate(LIS3MDL_DATARATE_155_HZ);
  lis3mdl.setRange(LIS3MDL_RANGE_4_GAUSS);

  // filter.begin(200);
  status.lsm_ok = true;
  return true;
}

// ----------------------------------------------------------------------- loop

void updateLSM() {
  sensors_event_t accel, gyro, temp, mag;
  if (!lsm6ds.getEvent(&accel, &gyro, &temp)) {
    status.lsm_ok = false;
    return;
  }
  if (!lis3mdl.getEvent(&mag)) {
    status.lsm_ok = false;
    return;
  }

  // ---- accelerometer (m/s² → g) ----
  sensors.accel1_x = accel.acceleration.x;
  sensors.accel1_y = accel.acceleration.y;
  sensors.accel1_z = accel.acceleration.z;

  // ---- gyroscope (rad/s, Adafruit already gives rad/s) ----
  sensors.gyro1_x = gyro.gyro.x;
  sensors.gyro1_y = gyro.gyro.y;
  sensors.gyro1_z = gyro.gyro.z;

  // ---- magnetometer — apply hard+soft iron correction ----
  sensors.mag1_x = mag.magnetic.x;
  sensors.mag1_y = mag.magnetic.y;
  sensors.mag1_z = mag.magnetic.z;

  // // ---- Madgwick update ----
  // // Adafruit gyro is already in rad/s so pass directly
  // filter.updateIMU(gx, gy, gz, ax, ay, az);  // start without mag to verify accel/gyro
  // // Once accel+gyro looks correct, switch to full update:
  // // filter.update(gx, gy, gz, ax, ay, az, mx, my, mz);

  // ---- output quaternion CSV ----
  // Serial.print(millis());      Serial.print(",");
  // Serial.print(filter.q0, 6); Serial.print(",");
  // Serial.print(filter.q1, 6); Serial.print(",");
  // Serial.print(filter.q2, 6); Serial.print(",");
  // Serial.print(filter.q3, 6); Serial.print(",");
  /* Display the results (acceleration is measured in m/s^2) */
}