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

LIS3DH myIMU( I2C_MODE, 0x18 ); //Default constructor is I2C, addr 0x19 = 25 = 0b0011001 We need and address of 0x18 = 24 = 0b0011000 b/c we pull that pin down

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(1000); //relax...
  Serial.println("Processor came out of reset.\n");
  
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
    Serial.println("Problem starting the sensor at 0x18.");
    while(1);
  }else{
    Serial.println("Starting the sensor at 0x18.");
    
  }

}

void loop()
{
  //Get all parameters
Serial.print("\nAccelerometer:\n");
  Serial.print(" X1 = ");
  Serial.println(myIMU.readFloatAccelX(), 4);
  Serial.print(" Y1 = ");
  Serial.println(myIMU.readFloatAccelY(), 4);
  Serial.print(" Z1 = ");
  Serial.println(myIMU.readFloatAccelZ(), 4);
  
  delay(300);
}
