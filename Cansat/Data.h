#pragma once

struct SensorData {
  float pressure = 0.0;
  float temperature = 0.0;
  float altitude = 0.0;

  float accel1_x = 0.0;
  float accel1_y = 0.0;
  float accel1_z = 0.0;

  float roll1 = 0.0;
  float pitch1 = 0.0;
  float yaw1 = 0.0;

  float accel2_x = 0.0;
  float accel2_y = 0.0;
  float accel2_z = 0.0;

  float roll2 = 0.0;
  float pitch2 = 0.0;
  float yaw2 = 0.0;

  float gps_lat = 0.0;
  float gps_lng = 0.0;
  float gps_alt = 0.0;
  int gps_satellites = 0;
  bool gps_fix = false;
};

struct SystemStatus {
  bool bmp_ok = false;
  bool gps_ok = false;
  bool lsm_ok = false;
  bool icm_ok = false;
  bool sd_ok = false;
  bool lora_ok = false;

  bool launched = false;
  bool landed = false;
};

struct MissionData {
  unsigned long bootTime = 0;
  unsigned long lastTelemetry = 0;
  unsigned long lastLog = 0;
  unsigned long lastBuzzer = 0;

  float maxAltitude = 0.0;
  float launchAltitude = 0.0;
};

extern SensorData sensors;
extern SystemStatus status;
extern MissionData mission;