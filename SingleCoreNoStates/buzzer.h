// Pick Buzzer Type
#define BuzzerType BuzzerPassive

// Buzzer identifiers
#define BuzzerBMP 1
#define BuzzerSD 2
#define BuzzerBNO 3
#define BuzzerCAM 4

// Time between identifiers (ms)
#define BuzzerDiff 500

// Buzzer Function Wrapper
#if BuzzerType == BuzzerActive
#define BuzzerON gpio_put(BuzzerPin, HIGH)
#define BuzzerOFF gpio_put(BuzzerPin, LOW)
#else
#define BuzzerON tone(BuzzerPin, BuzzerFreq)
#define BuzzerOFF noTone(BuzzerPin)
#endif

void PlayBuzzer(uint32_t time) {
  BuzzerON;
  sleep_ms(time);
  BuzzerOFF;
}

void LongBeep(int module) {
  for (int i = 0; i < module; i++) {
    PlayBuzzer(500);
    sleep_ms(250);
  }
  sleep_ms(BuzzerDiff);
}

void setup_complete() {
  PlayBuzzer(200);
  sleep_ms(100);
  for (int i = 0; i < 3; i++) {
    PlayBuzzer(100);
    sleep_ms(50);
  }
  PlayBuzzer(300);
  sleep_ms(350);
  PlayBuzzer(100);
  sleep_ms(250);
  PlayBuzzer(100);  
}

void SetupError(int module) {
  // Error in setup identifier (3 fast beeps)
  for (int i = 0; i < 3; i++) {
    PlayBuzzer(50);
    sleep_ms(50);
  }
  sleep_ms(BuzzerDiff);

  // Module identifier
  LongBeep(module);
}
