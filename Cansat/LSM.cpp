extern "C" {
  #include <Fusion.h>
}

#include "LSM.h"

Adafruit_LSM6DS3TRC lsm6ds;
Adafruit_LIS3MDL lis3mdl;
FusionAhrs ahrs;

// magnetometer hard-iron offsets + soft-iron scale (set by calibrateMag)
float mx_offset = 0, my_offset = 0, mz_offset = 0;
float mx_scale  = 1, my_scale  = 1, mz_scale  = 1;

static constexpr float FILTER_HZ    = 104.0f;
static constexpr float DT_SECONDS   = 1.0f / FILTER_HZ;
static constexpr float G_TO_MS2     = 9.80665f;

void calibrateMag() {
  Serial.println("==============================================");
  Serial.println("Mag calibration — rotate sensor in a slow");
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
    sensors_event_t event;
    lis3mdl.getEvent(&event);

    float mx = event.magnetic.x;
    float my = event.magnetic.y;
    float mz = event.magnetic.z;

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

  lsm6ds.setAccelRange(LSM6DS_ACCEL_RANGE_16_G);
  lsm6ds.setGyroRange(LSM6DS_GYRO_RANGE_250_DPS);
  lsm6ds.setAccelDataRate(LSM6DS_RATE_104_HZ);
  lsm6ds.setGyroDataRate(LSM6DS_RATE_104_HZ);

  lis3mdl.setPerformanceMode(LIS3MDL_ULTRAHIGHMODE);
  lis3mdl.setOperationMode(LIS3MDL_CONTINUOUSMODE);
  lis3mdl.setDataRate(LIS3MDL_DATARATE_155_HZ);
  lis3mdl.setRange(LIS3MDL_RANGE_4_GAUSS);

  FusionAhrsInitialise(&ahrs);

  FusionAhrsSettings settings = {
    .convention = FusionConventionNwu,
    .gain = 0.5f,
    .gyroscopeRange = 250.0f,          // matches your LSM6DS3 gyro range
    .accelerationRejection = 10.0f,
    .magneticRejection = 10.0f,
    .recoveryTriggerPeriod = (unsigned int)(5.0f * FILTER_HZ),
  };

  FusionAhrsSetSettings(&ahrs, &settings);

  calibrateMag();
  status.lsm_ok = true;
  return true;
}

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

  // store raw-ish engineering units
  sensors.accel1_x = accel.acceleration.x;
  sensors.accel1_y = accel.acceleration.y;
  sensors.accel1_z = accel.acceleration.z;

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

  sensors.accel1_x = accelEarth.axis.x * G_TO_MS2;
  sensors.accel1_y = accelEarth.axis.y * G_TO_MS2;
  sensors.accel1_z = accelEarth.axis.z * G_TO_MS2;

  sensors.roll1  = euler.angle.roll;
  sensors.pitch1 = euler.angle.pitch;
  sensors.yaw1   = euler.angle.yaw;
}