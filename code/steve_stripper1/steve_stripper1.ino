#include "LED_Strip_Functions.h"

#define DEBUG false
#define USE_ACCELEROMETER true
#define USE_POTENTIOMETER true

#define POT_PIN      A7
#define NUM_LEDS_IN_STRIP   54 //54 on steve's hood
#define PULSE_GAP_LOOPS     30
#define PULSE_LENGTH        5
#define MAX_LOOP_DELAY_MS  400
#define MIN_LOOP_DELAY_MS  10

int32_t loop_delay_ms = 100;


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
//  if(USE_ACCELEROMETER){
//    loop_delay_ms = loop_range * Get_percent_tilt_value();  
//    loop_delay_ms /= 100;
//    loop_delay_ms += MIN_LOOP_DELAY_MS;
//
//    if(loop_delay_ms > MAX_LOOP_DELAY_MS) {
//      loop_delay_ms = MAX_LOOP_DELAY_MS;
//    }else if ( loop_delay_ms < MIN_LOOP_DELAY_MS){
//      loop_delay_ms = MIN_LOOP_DELAY_MS;
//    }
//  }else 
  if(USE_POTENTIOMETER){
    loop_delay_ms = analogRead(POT_PIN); //1024 is max val
//    if(DEBUG) Serial.println(loop_delay_ms);
    loop_delay_ms *= loop_range;
    loop_delay_ms /= 1024;//re-normalize 
    loop_delay_ms += MIN_LOOP_DELAY_MS;
//    if(DEBUG) Serial.println(loop_delay_ms);
    if(loop_delay_ms > MAX_LOOP_DELAY_MS) {
      loop_delay_ms = MAX_LOOP_DELAY_MS;
    }else if ( loop_delay_ms < MIN_LOOP_DELAY_MS){
      loop_delay_ms = MIN_LOOP_DELAY_MS;
    }
//    if(DEBUG) Serial.println(loop_delay_ms);
  }
  
}

void addPulses(){
  static int ms_since_pulse = 0;
  static int loops_since_pulse = 0;
  
  if(USE_ACCELEROMETER){
    if(Got_motion_bump()){
      Serial.println("Bump");
      InitiatePulse( PULSE_LENGTH,  RANDOM_COLOR );
    }
  }
  else if(loops_since_pulse >= PULSE_GAP_LOOPS){
    InitiatePulse( PULSE_LENGTH,  generatePulseColor() );
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
  updateDelay();
  ProgressPattern();
  addPulses();
  PrepArray(led_array);
//  PowerSaveArray(led_array);
//  if(DEBUG) Serial.println("D");
  MirrorArray(led_array);
  lightUpStrip(led_array);
  delay(loop_delay_ms);
}
