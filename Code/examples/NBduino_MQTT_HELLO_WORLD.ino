/*
  Plug your NBduino SHIELD on the Arduino board,run this sketch, after loading is complete open the IDE serial monitor
  publish, over MQTT protocol, "Hello World!" as "payload" and the SHIELD's IMEI as "Topic"

  Arduino SHIELD: NBduino
  Arduino BOARD: ARDUINO UNO Rev3 (or similar)

  Author: Antonio Cafiero
  Date: 14/12/2018
*/
#include <NBduinoLibrary.h>

NBduino NBiot("54.76.137.235", 18224, "test", "test");
char Topic[32];

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Let's start!");
  delay(300);
  NBiot.begin();
  NBiot.reqIMEI(Topic);
  NBiot.connect(Topic);
}

void loop()
{
  NBiot.publish((const char*)Topic, "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789");
  delay(10000);
}