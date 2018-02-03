

// Simple strand test for Adafruit Dot Star RGB LED strip.
// This is a basic diagnostic tool, NOT a graphics demo...helps confirm
// correct wiring and tests each pixel's ability to display red, green
// and blue and to forward data down the line.  By limiting the number
// and color of LEDs, it's reasonably safe to power a couple meters off
// the Arduino's 5V pin.  DON'T try that with other code!
#include "Strip_math.h"

#define MAX_RAND 6
//This is a function that will generate a random color that is a bit biased.
//If you want the top end color to be white, just use 0xFFFFFF, but I think this looks slightly nicer

#define R_MASK 0x0000FF
#define G_MASK 0x00FF00
#define B_MASK 0xFF0000

#define UPSCALER  3 //number of virtual pixels per real pixel. This smooths things as the pixels move down the line but uses a bit more power
#define PULSE_SIZE  5 //This is the number of pixels that a single "Pulse" takes up when generated in the array

int  num_leds_in_strip ; // Number of LEDs in strip. Defaults to 10
int num_virtual_pixels; //number of virtual pixels we are tracking

#define DELAY_MAX   120
#define DELAY_MIN   10

int ms_delay;
int delay_increment;

uint32_t * virtual_arr;
uint32_t * pixels_arr;

//initialize the strip arrays that we're going to track
void InitializeStripMath(int num_leds){
    num_leds_in_strip = num_leds;
    num_virtual_pixels = num_leds * UPSCALER;
    virtual_arr = new uint32_t[ num_virtual_pixels ];
    pixels_arr = new uint32_t[num_leds_in_strip];
    ms_delay = ((DELAY_MAX - DELAY_MIN) / 2 ) + DELAY_MIN;
    delay_increment = (DELAY_MAX - DELAY_MIN) / num_virtual_pixels;
}

uint32_t ValsToColor(uint32_t r, uint32_t g, uint32_t b){
    r = r <<16;
    g = g <<8;
    return (r + g + b);
}

void printHex(uint32_t val){
  char charVal[6];
  sprintf(charVal, "%06X", val);
  Serial.println(charVal);
}

//returns an integer color that averages the pixels fed into it
uint32_t averagePixels(uint32_t* arr_in , long num_pixels){
  uint32_t ave_R= 0;
  uint32_t ave_B =0;
  uint32_t ave_G = 0;
  uint32_t temp;
  for(int j = 0; j < num_pixels; j++){
//    ave_R += ( arr_in[ j ]  & B_MASK )>>16;
//    ave_G += ( arr_in[ j ]  & G_MASK )>>8;
    temp = arr_in[ j ] ;
    temp = temp & R_MASK;
    ave_B += temp;
    temp = arr_in[ j ] ;
    temp = (temp & G_MASK )>>8;
    ave_G += temp;
    temp = arr_in[ j ] ;
    temp = (temp & B_MASK)>>16;
    ave_R += temp;
  }
  ave_R /= num_pixels;
  ave_G /= num_pixels;
  ave_B /= num_pixels;
  ave_R = ave_R <<16;
  ave_B = ave_B <<8;
  temp = ave_R + ave_G + ave_B;
  return temp;
}

//Compresses the array fed in into a smaller array
void AveDownSampleArrays( uint32_t * virt_arr, uint32_t * real_arr, int pixel_scaling, int real_len){
   int j;
   uint32_t ave;
   int offset;
   //loop through our virtual array
  for (int i = 0; i < real_len; i++){
    ave = 0;
    offset = i*pixel_scaling;
    if(pixel_scaling ==1){
        real_arr[i] = virt_arr[i];
    }
    else {
        real_arr[i] = averagePixels( &(virt_arr[offset]) , pixel_scaling);
    }
  }
}

//Update the delay between cycles to make light change speed how it comes out
int UpdateDelay(int loc_delay){
  static int loc_delay_increment = 2;
   loc_delay += loc_delay_increment;
   if(loc_delay > DELAY_MAX){
//    loc_delay_increment = -1*loc_delay_increment;
    loc_delay = DELAY_MIN;
   }else if(loc_delay < DELAY_MIN){
    loc_delay_increment = -1*loc_delay_increment;
   }
   Serial.println(loc_delay);
   return loc_delay;
}

//This drops a pulse into the strip
//if the color is set to 0 it makes a random color
void InitiatePulse( int num_pixels, uint32_t color){
  if(color ==0 ){
    color = RANDOM_COLOR;
  }
  for(int i = 0; i < num_pixels; i++){
    virtual_arr[i] += color;
  }
}

void MirrorArray(uint32_t* led_array){
    int half_num = num_leds_in_strip/2;
    int i;
    // Serial.println(num_leds_in_strip);
    for(i = 1; i < half_num; i++){
        led_array[num_leds_in_strip - i] = led_array[i];
        // Serial.println( i);
        // Serial.println("E");
        // Serial.println("D");
    }
}

//This just moves the pixels down the array
void StripPropagateBasic(){
    //move the entire array up one
  for(int i = num_virtual_pixels-1; i >0; i--){
    virtual_arr[i] = virtual_arr[i-1];
  }
 virtual_arr[0] = 0; //clear out the 0th pixel
}

void ProgressPattern(){
    StripPropagateBasic(  );
}

void PrepArray(uint32_t * output_array){
    AveDownSampleArrays( virtual_arr, output_array,UPSCALER, num_leds_in_strip);
}

//at most a neopixel draws 60mA - which corresponds to all 3 LEDs being at 255, or a total led value of 765
#define MAX_FLUX_VALUE 2550 //1mA -> 12.75 flux, use 200mA cap
//Currenly this is not working properly - it's jacking up some of the eges on the pixel groups
//FLAG SAH it may now be fixed. had to re-mask the colors after the math was performed
void PowerSaveArray( uint32_t * output_array ){
    uint32_t temp,r,g,b,total_flux,scaling_value = 0;
    for(int j = 0; j < num_leds_in_strip; j++){
        temp = output_array[ j ] ;
        temp = temp & R_MASK;
        total_flux += temp;
        temp = output_array[ j ] ;
        //isolate the green channel and downshift it
        temp = (temp & G_MASK )>>8;
        total_flux += temp;
        //isolate the blue channel and downshift it
        temp = output_array[ j ] ;
        temp = (temp & B_MASK)>>16;
        total_flux += temp;
    }
    Serial.print("Total Flux ");
    Serial.println(total_flux);
    //if we're below our flux cap, return
    if(total_flux < MAX_FLUX_VALUE) return;
    //mult the scaling value by 0xFF for easier math later on
    scaling_value = 0x0000FF* (uint32_t)MAX_FLUX_VALUE;
    scaling_value = scaling_value / total_flux;
    // scaling_value = 0xFF;
    Serial.print("Scaling Val ");
    Serial.println(scaling_value);
    for(int j = 0; j < num_leds_in_strip; j++){
        temp = output_array[ j ] ;
        r = temp & R_MASK;//isolate the red channel
        r *= scaling_value; //scale the red channel
        r /= 0xFF; //scale back down the red channel
        r = r & R_MASK; //isolate the red channel again
        g = (temp & G_MASK )>>8; //isolate and downshift the green channel
        g *= scaling_value;//scale the green
        // g /= 0xFF; //would scale back down but are already scaled up
        g = g & G_MASK; //re-isolate the green channel

        b = (temp & B_MASK)>>8; //isolate and downshift the blue channel. NOTE normally we'd do 16 but we'll do 8 to avoid having to re-upshift later!
        b *= scaling_value;
        // b = b<<16; normally we'd need to bump this back up 16 but we got cute. see NOTE above ^^
        b = b* B_MASK;
        temp = r + g + b;
        output_array[j] = temp;
    }


}

//this does a full pattern for us
void Run_test_pattern(uint32_t * output_array){
    static int ms_elapsed_since_delay = 0;
    static int counter = 0;
    StripPropagateBasic( );
    if(counter++ >=PULSE_SIZE*3 ) {
      InitiatePulse(PULSE_SIZE, 0);
      // counter = random(PULSE_SIZE*3);
      counter = 0;
    }
    // //down sample the virtual array into the pixels_arr
    AveDownSampleArrays( virtual_arr, output_array,UPSCALER, num_leds_in_strip);
    // // sendPixelArray(pixels_arr);
    ms_elapsed_since_delay += ms_delay;
    if(ms_elapsed_since_delay >100){
      ms_elapsed_since_delay = 0;
      ms_delay = UpdateDelay(ms_delay);
    }
    delay(ms_delay); //pause between loops
}
