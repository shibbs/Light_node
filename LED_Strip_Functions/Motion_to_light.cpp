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

float max_brightness = 100;
//value of cumulative accelerometer signal on all 3 channels that is considered high enough to register as a bump
float motion_threshold = 3;

//accelerometer variables
float x,y,z;
float raw_x, raw_y, raw_z =0;
//this sort of tunes the sensitivity of the lights
float low_threshold = max_brightness*.2;
int z_threshold = max_brightness/2;

LIS3DH myIMU( I2C_MODE, 0x18 ); //Default constructor is I2C, addr 0x19 = 25 = 0b0011001 We need and address of 0x18 = 24 = 0b0011000 b/c we pull that pin down




void Init_motion() {

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

  if(  myIMU.begin() != 0 )
  {
    if (SERIAL_DEBUG) Serial.println("Problem starting the sensor at 0x18.");
    while(1);
  }else{
    if (SERIAL_DEBUG) Serial.println("Starting the sensor at 0x18.");

  }

}

void read_acclerometer_vals( ){
  low_threshold = max_brightness*.2;
  z_threshold = max_brightness/2;
  //Get accelerometer Data and scale it to color values ( 0 to 256 )

  raw_x = myIMU.readFloatAccelX();
  raw_y = myIMU.readFloatAccelY();
  raw_z = myIMU.readFloatAccelZ();

  x = max_brightness* raw_x;
  y = max_brightness* raw_y;
  z = max_brightness* raw_z;
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

//return a usable color based on our motion info
uint32_t Get_color_from_motion(int max_brightness_to_use ){
    if(max_brightness_to_use > 255) max_brightness_to_use = 255;
    max_brightness = max_brightness_to_use;
    read_acclerometer_vals();
    //get red from x and green from y and use random blue
    return ((int)x)<<8 +  (((int)y )<<16) +  (random(max_brightness)); // Math magic
}

//returns 0 to 100 based on our tilt angle
int Get_percent_tilt_value(){
    int x_percent = (x*100) ;
    int y_percent = (y*100) ;
    return 100 - (( x_percent + y_percent) /2 );
}

//returns true if we saw a bunch of activity on the accelerometer since we last checked
bool Got_motion_bump(){
    static bool saw_bump_last = false;
    read_acclerometer_vals();
    //boost them to be positive
    if(raw_x<0) raw_x *=-1;
    if(raw_y<0) raw_y *=-1;
    if(raw_z<0) raw_z *=-1;
    //get the cumulative value for the sensors
    float cumulative_val = raw_x+raw_y+raw_z;
    if (SERIAL_DEBUG) Serial.println(cumulative_val);
    //check if we've seen enough signal to go over our threshold
    if(cumulative_val > motion_threshold){
        //we want to only send signal when it is new, so ignore if we saw a bump last time
        if(!saw_bump_last){
            if (SERIAL_DEBUG) Serial.println("New Bump");
            saw_bump_last = true;
            return true;
        }else{
            return false;
        }
    }
    // if (SERIAL_DEBUG) Serial.println("No Bump");
    //if we're below the threshold, then we didn't see shit
    saw_bump_last = false;
    return false;
}
