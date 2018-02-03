
void Init_motion();

//returns 0 to 100 based on our tilt angle
bool Get_percent_tilt_value();

//return a usable color based on our motion info
//maxx brightness is 0-255
uint32_t Get_color_from_motion(int max_brightness_to_use );

//returns true if we saw a bunch of activity on the accelerometer since we last checked
bool Got_motion_bump();
