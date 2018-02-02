#include "LED_Strip_Functions.h"

#define DEBUG false
#define USE_ACCELEROMETER false

#define NUM_LEDS_IN_STRIP   25
#define PULSE_GAP_LOOPS     1
#define MAX_LOOP_DELAY_MS  1000
#define MIN_LOOP_DELAY_MS  100
#define MAX_LED_VAL   100

int loop_delay_ms = 300;

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

int r_change = 3;
int b_change = 5;
int g_change = 1;

uint32_t generatePulseColor(){
  static int r = 100;
  static int g = 20;
  static int b = 50;
  r += r_change;
  b += b_change;
  g += g_change;
  if(r >= MAX_LED_VAL){
    r_change = -1*r_change;
    r =MAX_LED_VAL;
  }else if (r<=0){
    r_change = -1*r_change;
    r=0;
  }
  if(g >= MAX_LED_VAL){
    g_change = -1*g_change;
    g =MAX_LED_VAL;
  }else if (g<=0){
    g_change = -1*g_change;
    g=0;
  }
  if(b >= MAX_LED_VAL){
    b_change = -1*b_change;
    b =MAX_LED_VAL;
  }else if (b<=0){
    b_change = -1*b_change;
    b=0;
  }
  return ValsToColor(r,g,b);
  
//  //if we're using the accelerometer, use it to bias our color
//  if(USE_ACCELEROMETER){
//    return Get_color_from_motion(255);
//  }else{ //if we're not using the accelerometer, just generate a random color
//    return RANDOM_COLOR;
//  }
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

void print_arr(uint32_t * arr){
   Serial.println("Printing Array : ");
  for(int i = 0; i < NUM_LEDS_IN_STRIP; i++){
    Serial.println(arr[i]);
  }
}

void loop() {
  if(DEBUG) Serial.println("C");
  // put your main code here, to run repeatedly:
//  run_pattern(led_array);
//  updateDelay();
  ProgressPattern();
  addPulses();
//  print_arr(led_array);
  PrepArray(led_array);
//  PowerSaveArray(led_array);
//  if(DEBUG) Serial.println("D");
//  print_arr(led_array);
  lightUpStrip(led_array);
  delay(loop_delay_ms);
}
