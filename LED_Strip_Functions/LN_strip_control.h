#include "Arduino.h"
#include <Adafruit_NeoPixel.h>


//This is the code object that actually talks to the strip

void InitializeStrip(int num_LEDs);
void lightUpStrip(uint32_t * send_arr);
