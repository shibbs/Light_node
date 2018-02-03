//LED_STRIP_FUNCTIONS
#include "LN_strip_control.h"
#include "Strip_math.h"
#include "Motion_to_light.h"

//these Masks are values that we will use for isolating R,G,B channnls
#define R_MASK 0x0000FF
#define G_MASK 0x00FF00
#define B_MASK 0xFF0000
