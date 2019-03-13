/*
  After loading is complete open the IDE serial monitor and wait until the module attaching to the Network

  NOTE: LAUNCH THIS SKETCH JUST THE FIRST TIME YOU USE THE SHIELD!

  Arduino SHIELD: NBduino
  Arduino BOARD: ARDUINO UNO Rev3 (or similar)

  Author: Antonio Cafiero
  Date: 8/3/2019
*/
#include <NBduinoLibrary.h>

// Network settings 
const char* APN = "ep.inetd.gdsp"; //(Vodafone)
NBduino NBiot;


void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Let's start!");
  Serial.print("LEAVE THE MODULE ON FOR ABOUT ");
  Serial.println("15 MIN (JUST FOR THE FIRST POWER ON)");
  delay(300);
  NBiot.begin();
  NBiot.setAPN(APN);
  }

void loop() { 
}



