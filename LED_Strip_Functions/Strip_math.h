#include<stdio.h>
#include <Arduino.h>
#include <math.h>
#include <stdint.h>


#define RANDOM_COLOR { random(0xAAFFCC)}

//creates the strip math object that will track our light's progress
void InitializeStripMath(int num_leds);

//This drops a pulse into the strip of a given length if the color is set to 0 it makes a random color
void InitiatePulse(  int num_pixels, uint32_t color);

//this just runs the pattern over and over
void ProgressPattern();
void Run_test_pattern(uint32_t * output_array);

void SendToStrip(uint32_t * output_array);

void StripPropagateBasic();

void PrepArray(uint32_t * output_array);

void PowerSaveArray( uint32_t * output_array );
uint32_t ValsToColor(uint32_t r, uint32_t g, uint32_t b);
void MirrorArray(uint32_t* led_array);
