#include "LED_Strip_Functions.h"

#define DEBUG false
#define USE_ACCELEROMETER true

#define NUM_LEDS_IN_STRIP   36
#define PULSE_GAP_LOOPS     4
#define MAX_LOOP_DELAY_MS  1000
#define MIN_LOOP_DELAY_MS  100

int loop_delay_ms = 1000;

uint32_t led_array[NUM_LEDS_IN_STRIP];

void setup() {
  // put your setup code here, to run once:
  if(DEBUG) Serial.begin(57600);
  if(DEBUG) Serial.println("starting up Stripper1");
  delay(500);
  InitializeStripMath(NUM_LEDS_IN_STRIP);
  if(DEBUG) Serial.println("A");
  delay(500);
  InitializeStrip(NUM_LEDS_IN_STRIP);
  if(DEBUG) Serial.println("B");
  delay(500);
  if(USE_ACCELEROMETER ) Init_motion();
}

uint32_t generatePulseColor(){
  //if we're using the accelerometer, use it to bias our color
  if(USE_ACCELEROMETER){
    return Get_color_from_motion(255);
  }else{ //if we're not using the accelerometer, just generate a random color
    return RANDOM_COLOR;
  }
}

void updateDelay(){
  int loop_range = MAX_LOOP_DELAY_MS - MIN_LOOP_DELAY_MS;
  if(USE_ACCELEROMETER){
    loop_delay_ms = loop_range * Get_percent_tilt_value();  
    loop_delay_ms /= 100;
    loop_delay_ms += MIN_LOOP_DELAY_MS;

    if(loop_delay_ms > MAX_LOOP_DELAY_MS) {
      loop_delay_ms = MAX_LOOP_DELAY_MS;
    }else if ( loop_delay_ms < MIN_LOOP_DELAY_MS){
      loop_delay_ms = MIN_LOOP_DELAY_MS;
    }
  }
  
}

void addPulses(){
  static int ms_since_pulse = 0;
  static int loops_since_pulse = 0;
  if(loops_since_pulse >= PULSE_GAP_LOOPS){
    InitiatePulse( 1,  generatePulseColor() );
    ms_since_pulse = 0;
    loops_since_pulse =0;
    Serial.println("D");
  }
  ms_since_pulse += loop_delay_ms;
  loops_since_pulse ++;
  
}

void loop() {
  if(DEBUG) Serial.println("C");
  // put your main code here, to run repeatedly:
//  run_pattern(led_array);
//  updateDelay();
  ProgressPattern();
  addPulses();
  PrepArray(led_array);
//  PowerSaveArray(led_array);
//  if(DEBUG) Serial.println("D");
  lightUpStrip(led_array);
  delay(loop_delay_ms);
}
