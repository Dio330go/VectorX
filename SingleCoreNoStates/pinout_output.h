#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP3XX.h>
#include <Adafruit_GPS.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <pico/time.h>

#define DebugStatus false

// Pinout - Use GP {number}, do not use pin number

// BMP
#define BMP_I2C Wire1 // Wire = I2C0 Wire1 = I2C1
#define BMP_SDA 18
#define BMP_SCL 19
#define SEALEVELPRESSURE_HPA (1016.0) // Calibration

// BNO
#define BNO_I2C Wire 
#define BNO_SDA 20
#define BNO_SCL 21

// Buzzer
#define BuzzerPin 26
#define BuzzerFreq 4000
#define BuzzerActive 1
#define BuzzerPassive 0

// GPS
#define GPS_SERIAL Serial1
#define GPS_SERIAL_SPEED 9600 // default = 9600
#define GPS_TX 16
#define GPS_RX 17

// APC220 (antena)
#define Antena Serial2
#define AntenaBAUD 9600
#define AntenaTX 4
#define AntenaRX 5

// SD
#define SD_SPI SPI1
#define SD_TX 15
#define SD_RX 12
#define SD_SCK 14
#define SD_CS 13

/* CAM defs in Arducam.h (REMINDER)

#define SPI_PORT spi0
#define PIN_SCK  2
#define PIN_MOSI 3
#define PIN_MISO 0
#define PIN_CS   10 -> USELESS

#define I2C_PORT i2c1
#define PIN_SDA  18
#define PIN_SCL  19 */
#define CAM_CS 1
#define CAM_ON 1
#define CAM_OFF 0 
#define CAM_FREQ 1000

/*

  output

*/

#if DebugStatus
#define DataSerial Serial
#else
#define DataSerial Antena
#endif

// Frequency of datalogging: every x miliseconds
#define DATAFREQ 200

// Output
typedef struct 
{
  float time = 0;
  // BMP
  float temp = 0;
  float pressure = 0;
  float alt_bmp = 0;
  // GPS
  float lat = 0;
  char lat_c = 'X';
  float lon = 0;
  char lon_c = 'X';
  float alt_gps = 0;
  // BNO (XYZ)
  float euler[3];
  float accel[3];
  float linaccel[3];
  float magnetic[3];
  float gyro[3];
  float gravity[3];
} output;

// Floating -> número de algarismos após vírgula
#define BMP_FLOAT 1
#define GPS_FLOAT_LOCATION 5
#define GPS_FLOAT_OTHER 1

output data;

void PrintAntena() {
  // GPS data shortening
  float latitude = 0;
  float longitude = 0;

  // Latitude
  if (data.lat_c == 'N') {
    latitude = data.lat;
  }
  if (data.lat_c == 'S') {
    latitude = - data.lat;
  }

  // Longitude
  if (data.lon_c == 'E') {
    longitude = data.lon;
  }
  if (data.lon_c == 'W') {
    longitude = - data.lon;
  }
  
  // Temperature, Pressure, Altitude(BMP388), Yaw, Pitch, Roll, AccelX, AccelY, AccelZ, Latitude, Longitude, Altitude(GPS)
  if (latitude == 0 || longitude == 0) {
    DataSerial.printf("%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,0,0,0,%.1f", 
    data.temp, data.pressure, data.alt_bmp, 
    data.euler[0], data.euler[1], data.euler[2],
    data.accel[0], data.accel[1], data.accel[2],
    data.time
    );
  }
  else {
    DataSerial.printf("%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.5f,%.5f,%.1f,%.1f", 
    data.temp, data.pressure, data.alt_bmp, 
    data.euler[0], data.euler[1], data.euler[2],
    data.accel[0], data.accel[1], data.accel[2],
    latitude, longitude, data.alt_gps,
    data.time
    );
  }
  DataSerial.println();
}

void PrintSD(bool image) {
  File log = SD.open("datalog.csv", FILE_WRITE);

  if (log) {
    // GPS data shortening
    float latitude = 0;
    float longitude = 0;

    // Latitude
    if (data.lat_c == 'N') {
      latitude = data.lat;
    }
    if (data.lat_c == 'S') {
      latitude = - data.lat;
    }

    // Longitude
    if (data.lon_c == 'E') {
      longitude = data.lon;
    }
    if (data.lon_c == 'W') {
      longitude = - data.lon;
    }
    
    // Main data output
    char csvLine[512];
    int csvSize = snprintf(csvLine, 200, "%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.5f,%.5f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f", 
      data.time, data.temp, data.pressure, data.alt_bmp, 
      data.euler[0], data.euler[1], data.euler[2],
      data.accel[0], data.accel[1], data.accel[2],
      latitude, longitude, data.alt_gps,
      data.linaccel[0], data.linaccel[1], data.linaccel[2],
      data.magnetic[0], data.magnetic[1], data.magnetic[2],
      data.gyro[0], data.gyro[1], data.gyro[2],
      data.gravity[0], data.gravity[1], data.gravity[2]
    );
    log.write(csvLine, csvSize);

    // Image State
    if (image) log.printf(",imgTaken");
    
    // Endline
    log.println();
    log.close();
  } 
  else {
    DataSerial.println("FOPEN_FAIL");    
  }  
}
