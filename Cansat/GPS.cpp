#include "GPS.h"

Adafruit_GPS GPS(&Serial1);

bool initGPS() 
{
  Serial1.setRX(GPS_RX);
  Serial1.setTX(GPS_TX);
  Serial1.begin(9600);

  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
  // the parser doesn't care about other sentences at this time

  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate
  // For the parsing code to work nicely and have time to sort thru the data, and
  // print it out we don't suggest using anything higher than 1 Hz

  status.gps_ok = true;
  return true;
}

void updateGPS() {
  // Feed bytes from UART into the parser
  while (Serial1.available()) {
    GPS.read();
  }

  // Parse only when a full NMEA sentence arrived
  if (GPS.newNMEAreceived()) {
    if (!GPS.parse(GPS.lastNMEA())) {
      return;
    }
  }

  sensors.gps_fix = GPS.fix;

  if (GPS.fix) {
    sensors.gps_lat = GPS.latitudeDegrees;
    sensors.gps_lng = GPS.longitudeDegrees;
    sensors.gps_alt = GPS.altitude;
    sensors.gps_satellites = GPS.satellites;
  }
}