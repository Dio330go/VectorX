/* 

  Setup functions

*/

// Adafruit_BMP3XX bmp;
Adafruit_GPS GPS(&GPS_SERIAL);
// Adafruit_BNO055 bno = Adafruit_BNO055(55);

// BMP
// void SetupBMP() {
//   // Pins  
//   BMP_I2C.setSDA(BMP_SDA);
//   BMP_I2C.setSCL(BMP_SCL);

//   if (!bmp.begin_I2C(BMP3XX_DEFAULT_ADDRESS, &Wire1)) {
//     DataSerial.println("BMP_INIT_FAIL");
//     SetupError(BuzzerBMP);
//   }
  
//   // Set up oversampling and filter initialization
//   bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_16X);
//   bmp.setPressureOversampling(BMP3_OVERSAMPLING_16X);
//   bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
//   bmp.setOutputDataRate(BMP3_ODR_50_HZ);
// }

// GPS
void SetupGPS() {
  // Pins    
  GPS_SERIAL.setTX(GPS_TX);
  GPS_SERIAL.setRX(GPS_RX);
  GPS_SERIAL.begin(9600);
  GPS.begin(9600);
  
  // CFG
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_GGAONLY);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // AUMENTAR? DEFS in gps lib Adafruit_PMTK.h  
  sleep_ms(1000);
  GPS_SERIAL.println(PMTK_Q_RELEASE);
}

// APC220
// void SetupAntena() {
//   // Pins
//   Antena.setTX(AntenaTX);
//   Antena.setRX(AntenaRX);
//   Antena.begin(AntenaBAUD);
//   sleep_ms(500);
// }

// // SD
// void SetupSD() {
//   // Pins
//   SD_SPI.setTX(SD_TX);
//   SD_SPI.setRX(SD_RX);
//   SD_SPI.setSCK(SD_SCK);
//   SD_SPI.setCS(SD_CS);    

//   if (!SD.begin(SD_CS, SPI_FULL_SPEED, SD_SPI)) {
//     DataSerial.println("SD_INIT_FAIL");
//     SetupError(BuzzerSD);
//   }
// }

// // BNO
// void SetupBNO() {
//   // Pins
//   BNO_I2C.setSDA(BNO_SDA);
//   BNO_I2C.setSCL(BNO_SCL);

//   if (!bno.begin()) {
//     DataSerial.println("BNO_INIT_FAIL");
//     SetupError(BuzzerBNO);
//   }
// }

// // Buzzer
// void SetupBuzzer() {
//   gpio_init(BuzzerPin);
//   gpio_set_dir(BuzzerPin, GPIO_OUT);
// }


/* 

  Measure BMP + GPS

*/
void measure() {
  // Time
  data.time = (float) millis() / 1000;

  // // BMP read
  // data.temp = 0; data.pressure = 0; data.alt_bmp = 0;
  // if (bmp.performReading()) {
  //   data.temp = bmp.temperature;
  //   data.pressure = bmp.pressure / 100.0;
  //   data.alt_bmp = bmp.readAltitude(SEALEVELPRESSURE_HPA);                                    
  // }

  // GPS read
  data.lat = 0; data.lat_c = 'X';
  data.lon = 0; data.lon_c = 'X';
  data.alt_gps = 0; 
  if (GPS.fix) {
    data.lat = GPS.latitude; data.lat_c = GPS.lat;
    data.lon = GPS.longitude; data.lon_c = GPS.lon;
    data.alt_gps = GPS.altitude;      
  }

//   // BNO read 
//   sensors_event_t euler, accel, linaccel, magnetic, gyro, gravity;
//   bno.getEvent(&euler, Adafruit_BNO055::VECTOR_EULER);
//   bno.getEvent(&accel, Adafruit_BNO055::VECTOR_ACCELEROMETER);
//   bno.getEvent(&linaccel, Adafruit_BNO055::VECTOR_LINEARACCEL);
//   bno.getEvent(&magnetic, Adafruit_BNO055::VECTOR_MAGNETOMETER);
//   bno.getEvent(&gyro, Adafruit_BNO055::VECTOR_GYROSCOPE);    
//   bno.getEvent(&gravity, Adafruit_BNO055::VECTOR_GRAVITY);

//   data.euler[0] = euler.orientation.x; data.euler[1] = euler.orientation.y; data.euler[2] = euler.orientation.z;
//   data.accel[0] = accel.acceleration.x; data.accel[1] = accel.acceleration.y; data.accel[2] = accel.acceleration.z;
//   data.linaccel[0] = linaccel.acceleration.x; data.linaccel[1] = linaccel.acceleration.y; data.linaccel[2] = linaccel.acceleration.z;
//   data.magnetic[0] = magnetic.magnetic.x; data.magnetic[1] = magnetic.magnetic.y; data.magnetic[2] = magnetic.magnetic.z;
//   data.gyro[0] = gyro.gyro.x; data.gyro[1] = gyro.gyro.y; data.gyro[2] = gyro.gyro.z;  
//   data.gravity[0] = gravity.acceleration.x; data.gravity[1] = gravity.acceleration.y; data.gravity[2] = gravity.acceleration.z;
}