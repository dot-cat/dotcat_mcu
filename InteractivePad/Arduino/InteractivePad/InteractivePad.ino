/**************************************************************************/

/*
 * Sectors Layout:
 * | 0 | 3 |
 * | 1 | 4 |
 * | 2 | 5 |
 */

#define N_OF_SECTORS 6
#define LED_MAX_BRIGHTNESS 127
#define N_OF_WAVES 2

const uint8_t SECTOR_SENSORS[N_OF_SECTORS] = { A4, A5, A0, A2, A3, A1 };
const int SECTOR_LEDS[N_OF_SECTORS] = { 3, 5, 6, 11, 10, 9 };

uint8_t current_pwm_levels[N_OF_SECTORS];
int thresholds[N_OF_SECTORS];
uint8_t waves_left[N_OF_SECTORS];

/**************************************************************************/

int thresholds[N_OF_SECTORS];

/**************************************************************************/

void calibrate() {
  while (millis() < 1000) { // Compute average ambient light value
    for (uint8_t i = 0; i < N_OF_SECTORS; ++i) {
      thresholds[i] = (analogRead(SECTOR_SENSORS[i]) + thresholds[i]) >> 1;
    }
  }

  for (uint8_t i = 0; i < N_OF_SECTORS; ++i) {
    thresholds[i] += 20; // Set threshold to ambient + 20 points

    // DEBUG: Print current threshhold
    Serial.print( "threshold [" );
    Serial.print( i );
    Serial.print( "] = " );
    Serial.println( thresholds[i] );
  }
  
}

/**************************************************************************/

void activate_leds() { // Set current PWM levels for all LEDs
  for (uint8_t i = 0; i < N_OF_SECTORS; ++i) {
    analogWrite(SECTOR_LEDS[i], current_pwm_levels[i]);
  }
}

/**************************************************************************/

void set_led_levels() {
  
  for (uint8_t i = 0; i < N_OF_SECTORS; ++i) {
    int sensorValue = analogRead(SECTOR_SENSORS[i]);
    delay(1);        // delay in between reads for stability
    
    if (sensorValue > thresholds[i]) {
      current_pwm_levels[i] = LED_MAX_BRIGHTNESS;
      waves_left[i] = N_OF_WAVES;
    }
    else {
      if (current_pwm_levels[i] != 0 ) {
        --current_pwm_levels[i];
      }
      else {
        if (waves_left[i] != 0) {
          current_pwm_levels[i] = LED_MAX_BRIGHTNESS;
          --waves_left[i];
        }
      }
    }
  }
}

/**************************************************************************/

// the setup routine runs once when you press reset:
void setup() {
  
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

  for (uint8_t i = 0; i < N_OF_SECTORS; ++i) { // Setup outputs for LEDs of each sector
    pinMode(SECTOR_LEDS[i], OUTPUT);
  }

  calibrate();

  memset(current_pwm_levels, 0, sizeof(uint8_t) * N_OF_SECTORS);
  memset(waves_left, 0, sizeof(uint8_t) * N_OF_SECTORS);
  
}

/**************************************************************************/

// the loop routine runs over and over again forever:
void loop() {
  
  set_led_levels();
  
  activate_leds();

  delay(10);
  
}

/**************************************************************************/

