#include "BMP.h"

Adafruit_BMP3XX bmp;

bool initBMP() {
  Wire1.setSDA(BMP_SDA);
  Wire1.setSCL(BMP_SCL);
  Wire1.begin();

  if (!bmp.begin_I2C(BMP3XX_DEFAULT_ADDRESS, &Wire1)) {
    status.bmp_ok = false;
    return false;
  }

  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_50_HZ);

  status.bmp_ok = true;
  return true;
}

void updateBMP() {
  if (!bmp.performReading()) {
    status.bmp_ok = false;
    return;
  }

  sensors.temperature = bmp.temperature;
  sensors.pressure = bmp.pressure / 100.0;
  sensors.altitude = bmp.readAltitude(SEALEVELPRESSURE_HPA);
}