/******************************************************************************
ADCUsage.ino

Marshall Taylor @ SparkFun Electronics
Nov 16, 2016
https://github.com/sparkfun/LIS3DH_Breakout
https://github.com/sparkfun/SparkFun_LIS3DH_Arduino_Library

Description:
ADC pin reading.

This returns 10 bit values for the 3 analog in pins.  The ADC works a little different
than the arduino, in that the detectable range is centered around half-rail voltage and
is not rail to rail.  Connect a potentiometer between 3.3v and ground, and with the
wiper connected to a pin.

Also shown in this sketch is how the temperature is handled in the LIS3DH.
When temperature is enabled, ADC3 can be read to determine the temperature change
only, with a LSb worth 1 degree celsius.

Resources:
Uses Wire.h for i2c operation
Uses SPI.h for SPI operation
Either can be omitted if not used

Development environment specifics:
Arduino IDE 1.6.4
Teensy loader 1.23

Hardware connections:
Connect I2C SDA line to A4
Connect I2C SCL line to A5
Connect GND and 3.3v power to the IMU

This code is released under the [MIT License](http://opensource.org/licenses/MIT).

Please review the LICENSE.md file included with this example. If you have any questions 
or concerns with licensing, please contact techsupport@sparkfun.com.

Distributed as-is; no warranty is given.
******************************************************************************/

#include "SparkFunLIS3DH.h"
#include "Wire.h"
#include "SPI.h"
#include <Adafruit_NeoPixel.h>

#define SERIAL_DEBUG false

#define STRIP   5
#define ONBOARD 2
//comment out one of these lines!
#define STRIP_OR_ONBAORD  STRIP 
//#define STRIP_OR_ONBAORD  ONBOARD 

#if( STRIP_OR_ONBAORD == ONBOARD )
  //this is the setup for  using the LEDs on board
  #define LEDS_PIN            10
  #define NUMPIXELS      2
#else
  //these are the defines for a strip of LEDs
  #define LEDS_PIN            4
  #define NUMPIXELS      10
#endif

bool leds_on = false;
int loop_delay_ms = 300;

#define POT_PIN      A7
int pixel_delay_ms = 10;
// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, LEDS_PIN, NEO_GRB + NEO_KHZ800);

float max_brightness = 100;

//accelerometer variables
float x,y,z;
//this sort of tunes the sensitivity of the lights
float low_threshold = max_brightness*.2;
int z_threshold = max_brightness/2;
LIS3DH myIMU( I2C_MODE, 0x18 ); //Default constructor is I2C, addr 0x19 = 25 = 0b0011001 We need and address of 0x18 = 24 = 0b0011000 b/c we pull that pin down

void setup() {
  // put your setup code here, to run once:
  if (SERIAL_DEBUG) Serial.begin(57600);
  delay(1000); //relax...
  if (SERIAL_DEBUG) Serial.println("Processor came out of reset.\n");

  //set up our RGB LEDs
  pinMode(LED_BUILTIN, OUTPUT);
  pixels.begin(); // This initializes the NeoPixel library.

  //Set up the Accelerometer
  myIMU.settings.adcEnabled = 0;
  //Note:  By also setting tempEnabled = 1, temperature data is available
  //on ADC3.  Temperature *differences* can be read at a rate of
  //1 degree C per unit of ADC3
  myIMU.settings.tempEnabled = 0;
  myIMU.settings.accelSampleRate = 50;  //Hz.  Can be: 0,1,10,25,50,100,200,400,1600,5000 Hz
  myIMU.settings.accelRange = 2;      //Max G force readable.  Can be: 2, 4, 8, 16
  myIMU.settings.xAccelEnabled = 1;
  myIMU.settings.yAccelEnabled = 1;
  myIMU.settings.zAccelEnabled = 1;
  
  //Call .begin() to configure the IMU
 
  if(  myIMU.begin() != 0 )
  {
    if (SERIAL_DEBUG) Serial.println("Problem starting the sensor at 0x18.");
    while(1);
  }else{
    if (SERIAL_DEBUG) Serial.println("Starting the sensor at 0x18.");
    
  }

}

#define Z_THRESH -.5

bool check_for_tilt(){
  
  z = myIMU.readFloatAccelZ();
  Serial.print(" Z1 = ");
  Serial.println(z, 4);
  if(z > Z_THRESH)
    return true;
  return false;
}

int r = 100;
int g = 20;
int b = 50;



int r_change = 3;
int b_change = 5;
int g_change = 1;

void generatePulseColor(){
 

  r += r_change;
  b += b_change;
  g += g_change;
  if(r >= max_brightness){
    r_change = -1*r_change;
    r =max_brightness;
  }else if (r<=0){
    r_change = -1*r_change;
    r=0;
  }
  if(g >= max_brightness){
    g_change = -1*g_change;
    g =max_brightness;
  }else if (g<=0){
    g_change = -1*g_change;
    g=0;
  }
  if(b >= max_brightness){
    b_change = -1*b_change;
    b =max_brightness;
  }else if (b<=0){
    b_change = -1*b_change;
    b=0;
  }
//  return ValsToColor(r,g,b);
  
}

void read_acclerometer_vals(){
  low_threshold = max_brightness*.2;
  z_threshold = max_brightness/2;
  //Get accelerometer Data and scale it to color values ( 0 to 256 )
  x = max_brightness* myIMU.readFloatAccelX();
  y = max_brightness* myIMU.readFloatAccelY();
  z = max_brightness* myIMU.readFloatAccelZ();
  //convert the vals into useful ranges
  if(x < 0) x = -1 * x;
  if(y < 0) y = - 1 * y;
  if(z < 0) z = -1 * z;
  z = max_brightness - z;
  if(x < low_threshold) x = 0;
  if(y < low_threshold) y = 0;
  if(z < low_threshold) z = 0;
  
  if (SERIAL_DEBUG) {
    Serial.print("\nAccelerometer:\n");
    Serial.print(" X1 = ");
    Serial.println(x, 4);
    Serial.print(" Y1 = ");
    Serial.println(y, 4);
    Serial.print(" Z1 = ");
    Serial.println(z, 4);
  }
  
}

void setLEDs(int red, int green, int blue){
    // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.
  int sensorValue = 0;
  for(int i=0;i<NUMPIXELS;i++){
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(red, green, blue)); // Moderately bright green color.
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(pixel_delay_ms); // Delay for a period of time (in milliseconds).
  }
}

uint32_t color;
void loop()
{
  int num_loops = 0;

  
  //if the jar is tilted for a certain amount of time, toggle the LED state 
  while( check_for_tilt() ){
    delay(100);
    num_loops ++;
    //use exact b/c once we trigger we want to stay in that state until released
    if(num_loops == 5){
      leds_on = !leds_on;
      if(!leds_on){
          if (SERIAL_DEBUG) {Serial.println("turning off");}
          setLEDs(0,0,0);
      }else{
        if (SERIAL_DEBUG) {Serial.println("turning on");}
        setLEDs(r,g,b);
      }
    }
  }
  if(leds_on){
    generatePulseColor();
    max_brightness = analogRead(POT_PIN) >> 2; //range is 0-1024 normally do divide by 4
    setLEDs(r,g,b);
    delay(loop_delay_ms);
  }else{
    //do nothing b/c LEDs are off
    
  }
  
  
  //scale to 0-256
  max_brightness = analogRead(POT_PIN) / 4;
//  setLEDs(x,y,z);
  delay(50);
}
