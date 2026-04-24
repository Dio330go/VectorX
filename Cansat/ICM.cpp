extern "C" {
  #include <Fusion.h>
}

#include "ICM.h"

Adafruit_ICM20948 icm;
static FusionAhrs ahrs;
// uint16_t measurement_delay_us = 65535; // Delay between measurements for testing

// magnetometer hard-iron offsets + soft-iron scale (set by calibrateMag)
static float mx_offset = -27.98, my_offset = -103.97, mz_offset = 3.23;
static float mx_scale  = 1.20, my_scale  = 0.95, mz_scale  = 0.89;

static constexpr float FILTER_HZ    = 104.0f;
static constexpr float DT_SECONDS   = 1.0f / FILTER_HZ;
static constexpr float G_TO_MS2     = 9.80665f;

static void calibrateMag() {
  Serial.println("==============================================");
  Serial.println("Mag calibration — ICM20948");
  Serial.println("figure-8 covering ALL orientations for 20s.");
  Serial.println("Starting in 3 seconds...");
  Serial.println("==============================================");
  delay(3000);

  float mx_min =  1e9, my_min =  1e9, mz_min =  1e9;
  float mx_max = -1e9, my_max = -1e9, mz_max = -1e9;

  unsigned long start     = millis();
  unsigned long duration  = 20000;
  unsigned long lastPrint = 0;

  while (millis() - start < duration) {
    sensors_event_t accel, gyro, temp, mag;
    icm.getEvent(&accel, &gyro, &temp, &mag);

    float mx = mag.magnetic.x;
    float my = mag.magnetic.y;
    float mz = mag.magnetic.z;

    if (mx < mx_min) mx_min = mx;
    if (mx > mx_max) mx_max = mx;
    if (my < my_min) my_min = my;
    if (my > my_max) my_max = my;
    if (mz < mz_min) mz_min = mz;
    if (mz > mz_max) mz_max = mz;

    if (millis() - lastPrint > 1000) {
      Serial.print("Time left: ");
      Serial.print((duration - (millis() - start)) / 1000);
      Serial.println("s");
      lastPrint = millis();
    }
    delay(10);
  }

  mx_offset = (mx_max + mx_min) / 2.0f;
  my_offset = (my_max + my_min) / 2.0f;
  mz_offset = (mz_max + mz_min) / 2.0f;

  float mx_range = (mx_max - mx_min) / 2.0f;
  float my_range = (my_max - my_min) / 2.0f;
  float mz_range = (mz_max - mz_min) / 2.0f;
  float avg_range = (mx_range + my_range + mz_range) / 3.0f;

  if (mx_range != 0) mx_scale = avg_range / mx_range;
  if (my_range != 0) my_scale = avg_range / my_range;
  if (mz_range != 0) mz_scale = avg_range / mz_range;

  Serial.println("--- Mag calibration results ---");
  Serial.print("Offsets  X: "); Serial.print(mx_offset);
  Serial.print("  Y: ");        Serial.print(my_offset);
  Serial.print("  Z: ");        Serial.println(mz_offset);
  Serial.print("Scales   X: "); Serial.print(mx_scale);
  Serial.print("  Y: ");        Serial.print(my_scale);
  Serial.print("  Z: ");        Serial.println(mz_scale);
  Serial.println("Mag calibration done.");
}

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
  icm.setMagDataRate(AK09916_MAG_DATARATE_100_HZ); // SINGLE 10Hz 20Hz 50Hz 100Hz
  icm.setAccelRateDivisor(10);
  icm.setGyroRateDivisor(10);

  // uint16_t accel_divisor = icm.getAccelRateDivisor(); 
  // float accel_rate = 1125 / (1.0 + accel_divisor); // Hz
  // uint8_t gyro_divisor = icm.getGyroRateDivisor();
  // float gyro_rate = 1100 / (1.0 + gyro_divisor); // Hz
  FusionAhrsInitialise(&ahrs);

  FusionAhrsSettings settings = {
    .convention = FusionConventionNwu,
    .gain = 0.5f,
    .gyroscopeRange = 2000.0f,          // matches your LSM6DS3 gyro range
    .accelerationRejection = 10.0f,
    .magneticRejection = 10.0f,
    .recoveryTriggerPeriod = (unsigned int)(5.0f * FILTER_HZ),
  };

  FusionAhrsSetSettings(&ahrs, &settings);
  // calibrateMag();

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
    status.icm_ok = false;
    return;
  }

  // Fusion expects accelerometer in g
  const float ax = accel.acceleration.x / G_TO_MS2;
  const float ay = accel.acceleration.y / G_TO_MS2;
  const float az = accel.acceleration.z / G_TO_MS2;

  // Adafruit gyro is rad/s, Fusion expects deg/s
  const float gx = gyro.gyro.x * RAD_TO_DEG;
  const float gy = gyro.gyro.y * RAD_TO_DEG;
  const float gz = gyro.gyro.z * RAD_TO_DEG; 

  // Apply magnetometer calibration
  const float mx = (mag.magnetic.x - mx_offset) * mx_scale;
  const float my = (mag.magnetic.y - my_offset) * my_scale;
  const float mz = (mag.magnetic.z - mz_offset) * mz_scale;

  const FusionVector gyroscope = {
    .axis = { gx, gy, gz }
  };

  const FusionVector accelerometer = {
    .axis = { ax, ay, az }
  };

  const FusionVector magnetometer = {
    .axis = { mx, my, mz }
  };

  FusionAhrsUpdate(&ahrs, gyroscope, accelerometer, magnetometer, DT_SECONDS);

  FusionQuaternion q = FusionAhrsGetQuaternion(&ahrs);
  const FusionEuler euler =
      FusionQuaternionToEuler(q);

  // Convert quaternion to rotation matrix
  const FusionMatrix rotation = FusionQuaternionToMatrix(q);

  // Rotate body-frame accel into Earth/global frame
  FusionVector accelEarth = FusionMatrixMultiply(rotation, accelerometer);

  accelEarth.axis.z -= 1.0f;

  sensors.accel2_x = accelEarth.axis.x * G_TO_MS2;
  sensors.accel2_y = accelEarth.axis.y * G_TO_MS2;
  sensors.accel2_z = accelEarth.axis.z * G_TO_MS2;

  sensors.roll2  = euler.angle.roll;
  sensors.pitch2 = euler.angle.pitch;
  sensors.yaw2   = euler.angle.yaw;
}