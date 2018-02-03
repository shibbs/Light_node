## Steps to check PCB is working ##

0) Open Arduino IDE

1) Add Additional board URL: ( steps below from https://learn.adafruit.com/add-boards-arduino-v164/setup)
-go to arduino→preferences
-copy and paste link https://adafruit.github.io/arduino-board-index/package_adafruit_index.json into Additional Board Manager URLS

2) Install Boards: (steps below from https://learn.adafruit.com/add-boards-arduino-v164/installing-boards)
-go to tools->board->board manager
-switch type to Contributed
-click adafruit AVR boards
-click install

3) Select new board:
-tools→ boards→ “Pro Trinket 5V 16MHz
-tools→programmer→USBtinyISP

4) Check bootloader on-board LEDs:
-open code/light_node_test.ino
-Upload light_node_test. Make sure #define STRIP_OR_ONBAORD is uncommented and #define STRIP_OR_ONBAORD STRIP is commented
-it should light up.
-If you see,
avrdude: Error: Could not find USBtiny device (0x1781/0xc9f)
the bootloader isn’t working and you need Stephen’s help

5) Check accelerameter:
-clone https://github.com/sparkfun/SparkFun_LIS3DH_Arduino_Library into ~/Documents/Arduino/libraries
-open code/Light_Node_Accel_To_Light
-upload Light_Node_Accel_To_Light
-shake that shit

6) To use the libraries that have been set up to make the code easier to write and more readable you'll need to
set up a symbolic link from the LED_Strip_Functions folder into your Arduino Libraries folder. This is because Arduino is annoying and  requires all libraries/external code that is called to be housed within the Arduino/libraries folder.
To do this you'll need to run your own version of the below command to create a "symbolic link". this works for mac and linux, to do it for windows you'll need St. Google

Steve's command to call : ln -s ~/Documents/projects/Light_node/LED_Strip_Functions ~/Documents/Arduino/libraries/
To modify this, the first section "~/Documents/projects/Light_node/LED_Strip_Functions" is the path to the folder that your original code is sitting in
The second section "~/Documents/Arduino/libraries/" is the path to your arduino libraries folder. You probably don't need to modify this
