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

bool gesture_started = false;

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
  bool no_interrupts = true;
  
  for (uint8_t i = 0; i < N_OF_SECTORS; ++i) {
    int sensorValue = analogRead(SECTOR_SENSORS[i]);
    delay(1);        // delay in between reads for stability
    
    if (sensorValue > thresholds[i]) {
      current_pwm_levels[i] = LED_MAX_BRIGHTNESS;

      no_interrupts = false;
      gesture_started = true;
    }
    else {
      if (current_pwm_levels[i] != 0 ) {
        --current_pwm_levels[i];
      }
    }
  }

  if (gesture_started && no_interrupts) {
    dispatch_gesture();
    gesture_started = false;
  }

}

/**************************************************************************/

void dispatch_gesture() {
  

  //if (gesture_started) {
    if (current_pwm_levels[0] < current_pwm_levels[1] 
     && current_pwm_levels[1] < current_pwm_levels[2]
     && current_pwm_levels[0] > 0
    ) {
      Serial.println( "right" );
    }
    else if (current_pwm_levels[3] < current_pwm_levels[4] 
     && current_pwm_levels[4] < current_pwm_levels[5]
     && current_pwm_levels[3] > 0
    ) {
      Serial.println( "right" );
    }

    else if (current_pwm_levels[0] > current_pwm_levels[1] 
     && current_pwm_levels[1] > current_pwm_levels[2]
     && current_pwm_levels[2] > 0
    ) {
      Serial.println( "left" );
    }

    else if (current_pwm_levels[3] > current_pwm_levels[4] 
     && current_pwm_levels[4] > current_pwm_levels[5]
     && current_pwm_levels[5] > 0
    ) {
      Serial.println( "left" );
    }

   // gesture_started = false;
  //}
  
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
  
}

/**************************************************************************/

// the loop routine runs over and over again forever:
void loop() {
  
  set_led_levels();
  
  activate_leds();

  

  delay(10);
  
}

/**************************************************************************/

