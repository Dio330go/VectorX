#include "Data.h"
#include "Sensors.h"
#include "Constants.h"
#include "GPS.h"
// #include <Adafruit_GPS.h>
// #include <SoftwareSerial.h>
// #define GPS_TX 16      // 21
// #define GPS_RX 17      // 22

// #include "Mission.h"

SensorData sensors;
SystemStatus status;
MissionData mission;

// SoftwareSerial mySerial(GPS_TX, GPS_RX);
// Adafruit_GPS GPS(&mySerial);

// #define GPSECHO  true

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(LED_BUILTIN, LOW);
  delay(STARTING_DELAY);
  digitalWrite(LED_BUILTIN, HIGH); // reached setup start

  unsigned long start = millis();
  while (!Serial && millis() - start < 3000) {}

  Serial.println("Initialized");

  // if (!initSensors()) {
    // if (!status.bmp_ok) Serial.println("BMP Wrong");
    // if (!status.sd_ok) Serial.println("SD Wrong");
    // if (!status.lsm_ok) Serial.println("LSM Wrong");
    // if (!status.icm_ok) Serial.println("ICM Wrong");
    // if (!status.gps_ok) Serial.println("GPS Wrong");
    // if (!status.lora_ok) Serial.println("Lora Wrong");
  // }

  // GPS.begin(9600);

  // GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  // mySerial.println(PMTK_Q_RELEASE);

  initGPS();
  initSensors();
  digitalWrite(LED_BUILTIN, LOW);
}

// uint32_t timer = millis();
void loop() {
  updateGPS();

  updateSensors();
}