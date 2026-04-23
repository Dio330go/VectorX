#include <SPI.h>
#include <RadioLib.h>

#define LORA_SCK   18
#define LORA_MISO  16
#define LORA_MOSI  19
#define LORA_NSS   17
#define LORA_DIO0  20

const float SEALEVELPRESSURE_HPA = 1013.25;

SX1278 radio = new Module(LORA_NSS, LORA_DIO0, RADIOLIB_NC, RADIOLIB_NC);

// flag to indicate that a packet was received
volatile bool receivedFlag = false;


void setFlag(void) {
  receivedFlag = true;
}

void setup() {
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(LED_BUILTIN, LOW);
  delay(10000);
  digitalWrite(LED_BUILTIN, HIGH);

  SPI.setSCK(LORA_SCK);
  SPI.setRX(LORA_MISO);
  SPI.setTX(LORA_MOSI);
  SPI.begin();

  Serial.print(F("[SX1278] Initializing ... "));
  int state = radio.begin();
  if(state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while(true) { delay(10); }
  }

  radio.setPacketReceivedAction(setFlag);

  Serial.print(F("[SX1278] Starting to listen ... "));
  state = radio.startReceive();
  if(state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while(true) { delay(10); }
  }
}

void loop() {
  if(receivedFlag) {
    receivedFlag = false;
    
    uint8_t packet[16];
    int state = radio.readData(packet, 16);
    
    uint16_t pressure = (packet[0] << 8) | packet[1];
    float pressure_f = pressure / 100.0 + SEALEVELPRESSURE_HPA;

    int8_t temp = (int8_t)packet[2];
    float temp_f = temp / 2.0;

    int16_t ax = (packet[3] << 8) | packet[4];
    int16_t ay = (packet[5] << 8) | packet[6];
    int16_t az = (packet[7] << 8) | packet[8];

    int8_t angX = (int8_t)packet[9];
    int8_t angY = (int8_t)packet[10];
    int8_t angZ = (int8_t)packet[11];

    int16_t gpsX = (packet[12] << 8) | packet[13];
    int16_t gpsY = (packet[14] << 8) | packet[15];

    if(state == RADIOLIB_ERR_NONE) {
      Serial.println(F("[SX1278] Received packet!"));

      // Serial.print(F("[SX1278] Data:\t\t"));
      // Serial.println(str);

      Serial.print(F("[SX1278] RSSI:\t\t"));
      Serial.print(radio.getRSSI());
      Serial.println(F(" dBm"));

      Serial.print(F("[SX1278] SNR:\t\t"));
      Serial.print(radio.getSNR());
      Serial.println(F(" dB"));

      Serial.print(F("[SX1278] Frequency error:\t"));
      Serial.print(radio.getFrequencyError());
      Serial.println(F(" Hz"));

    } else if(state == RADIOLIB_ERR_CRC_MISMATCH) {
      Serial.println(F("[SX1278] CRC error!"));
    } else {
      Serial.print(F("[SX1278] Failed, code "));
      Serial.println(state);
    }

    state = radio.startReceive();
    if(state != RADIOLIB_ERR_NONE) {
      Serial.print(F("[SX1278] Failed to restart receive, code "));
      Serial.println(state);
    }
  }
}