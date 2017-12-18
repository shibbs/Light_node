// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define STRIP   1
#define ONBOARD 2
//comment out one of these lines!
//#define STRIP_OR_ONBOARD  STRIP
#define STRIP_OR_ONBOARD  ONBOARD



// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1

#if( STRIP_OR_ONBOARD == ONBOARD )
  //this is the setup for  using the LEDs on board
  #define LEDS_PIN            10
  #define NUMPIXELS      2
#else
  //these are the defines for a strip of LEDs
  #define LEDS_PIN            4
  #define NUMPIXELS      10
#endif

#define POT_PIN      A7

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, LEDS_PIN, NEO_GRB + NEO_KHZ800);

int delayval = 500; // delay for half a second

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
//#if defined (__AVR_ATtiny85__)
//  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
//#endif
  // End of trinket special code
  pinMode(LED_BUILTIN, OUTPUT);
  pixels.begin(); // This initializes the NeoPixel library.
}

void loop() {

  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.
  int sensorValue = 0;
  for(int i=0;i<NUMPIXELS;i++){
    
    sensorValue = analogRead(POT_PIN);
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(150,150,sensorValue)); // Moderately bright green color.
//    strip.setPixelColor(i, strip.Color(150,150,sensorValue)); // Moderately bright green color.

//    strip.show();
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(delayval); // Delay for a period of time (in milliseconds).

  }
}
