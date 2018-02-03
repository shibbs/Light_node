#include "LN_strip_control.h"

#define LEDS_PIN            4
int NUM_PIXELS = 0;
uint32_t * curr_pixels;

Adafruit_NeoPixel strip ;//=Adafruit_NeoPixel(10, LEDS_PIN, NEO_GRB + NEO_KHZ800);;

void InitializeStrip(int num_LEDs){
    strip = Adafruit_NeoPixel(num_LEDs, LEDS_PIN, NEO_GRB + NEO_KHZ800);
    NUM_PIXELS     = num_LEDs;
    curr_pixels = new uint32_t[NUM_PIXELS];
    strip.begin();
}

void lightUpStrip(uint32_t * send_arr){
  //loop over all pixels in array
  // Serial.println("Setting Strip");

  //for some reason this is not working for shit, so I've pulled it for now. Would like to resolve some time since we're currently just spinning our wheels on this
  for (int i = 0; i < NUM_PIXELS; i++){
    // //if pixel changed, then update it and update our static array
    //For some reason the below chunk of code is inserting random colors into the output. Can't figure out hy
    // if(curr_pixels[i] != send_arr[i]){
    //   curr_pixels[i] = send_arr[i];
    //   Serial.println(send_arr[i]);
    //   strip.setPixelColor(i, send_arr[i]);
    // }
    strip.setPixelColor(i, send_arr[i]);
  }
  strip.show();                     // Refresh strip
}
