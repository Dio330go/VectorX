#include "pinout_output.h"
#include "buzzer.h"
#include "setup_measure.h"


// Read/TX timings
uint32_t timer_data = millis();
uint32_t timer_cam = millis();

void setup() { 
  Serial.begin(9600);
  SetupGPS();
  SetupAntena();    
  SetupBNO(); 
  SetupBMP();
  SetupSD();      
  setup_complete();

  // SPI speedup
  spi_set_baudrate(spi0, 60000000);
}

void loop() {  

  // Image
  bool imageTaken = false;
  
  // Timings and reads
  if (millis() - timer_data > DATAFREQ) { 
    timer_data = millis();             
    
    measure();        
    
    // Cam take pic
    if (millis() - timer_cam > CAM_FREQ) {
      timer_cam = millis();

      // Image filename setup "milisecond.jpg"
      char image_filename[15];
      snprintf(image_filename, 15, "%u.jpg", timer_data);

      if (!CaptureSD(camera, image_filename)) {
        DataSerial.println("CAM_CAPTURE_FAIL");
      }
      else {
        imageTaken = true;
      }
    }
    
    // DATALOG     
    PrintAntena();
    PrintSD(imageTaken);        
  }
}
void setup1() {
  delay(10000);
  PlayBuzzer(300);
}

void loop1() {
  char c = GPS.read();

  if (GPS.newNMEAreceived()) {
    if (!GPS.parse(GPS.lastNMEA())) return; 
  }
}
