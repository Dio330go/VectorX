#include "GPS.h"

SoftwareSerial mySerial(GPS_TX, GPS_RX);
Adafruit_GPS GPS(&mySerial);

bool initGPS() {
  GPS.begin(9600);
  delay(2000);

  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  mySerial.println(PMTK_Q_RELEASE);

  status.gps_ok = true;
  return true;
 }

uint32_t timer = millis();
void updateGPS() {
  GPS.read();
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
    //Serial.println(GPS.lastNMEA());   // this also sets the newNMEAreceived() flag to false

    if (!GPS.parse(GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
      return;  // we can fail to parse a sentence in which case we should just wait for another
  }
  if (millis() - timer > 1000) {
    timer = millis(); // reset the timer
    Serial.print("gps_lat = ");
    Serial.print(sensors.gps_lat);
    Serial.print("    gps_lng = ");
    Serial.print(sensors.gps_lng);
    Serial.print("    gps_satellites = ");
    Serial.print(sensors.gps_satellites);
    Serial.print("    gps_fix = ");
    Serial.println(sensors.gps_fix);
  }

  sensors.gps_fix = GPS.fix;

  if (GPS.fix) {
    sensors.gps_lat = GPS.latitude;
    sensors.gps_lng = GPS.longitude;
    sensors.gps_alt = GPS.altitude;
    sensors.gps_satellites = GPS.satellites;
  } else {
    sensors.gps_satellites = GPS.satellites;
  }
}