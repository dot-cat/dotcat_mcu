/**************************************************************************/

/*
 * Sectors Layout:
 * | 0 | 3 |
 * | 1 | 4 |
 * | 2 | 5 |
 */

#define N_OF_SECTORS 6
#define LED_MAX_BRIGHTNESS 127

const uint8_t SECTOR_SENSORS[N_OF_SECTORS] = { A4, A5, A0, A2, A3, A1 };
const int SECTOR_LEDS[N_OF_SECTORS] = { 3, 5, 6, 11, 10, 9 };

uint8_t current_pwm_levels[N_OF_SECTORS];

//int8_t sensor_order[N_OF_SECTORS];


/**************************************************************************/

int threshold;

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
    
    if (sensorValue > threshold + 20) {
      current_pwm_levels[i] = LED_MAX_BRIGHTNESS;
    }
    else {
      if (current_pwm_levels[i] != 0 ) {
        --current_pwm_levels[i];
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

  pinMode(2, INPUT); // DEBUG: Light up LED 1 on button press

  while (millis() < 1000) { // Compute average ambient light value
    threshold = (analogRead(A4) + threshold) >> 1;
  }

  threshold += 20; // Set threshold to ambient + 20 points

  // DEBUG: Print current threshhold
  Serial.print( "threshold = " );
  Serial.println( threshold );

  //memset(sensor_order, 0, sizeof(int8_t) * N_OF_SECTORS);
  memset(current_pwm_levels, 0, sizeof(uint8_t) * N_OF_SECTORS);
}

/**************************************************************************/

// the loop routine runs over and over again forever:
void loop() {
  set_led_levels();
  
//  if (digitalRead(2) == LOW) { // DEBUG: Light up LED 1 on button press
//    current_pwm_levels[1] = LED_MAX_BRIGHTNESS;
//  }
  
  activate_leds();

  
  delay(10);
}

/**************************************************************************/

