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
