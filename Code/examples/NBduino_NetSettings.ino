/*
  Plug your NBduino SHIELD on the Arduino board, run this sketch to set the Network Settings;
  After loading is complete open the IDE serial monitor and wait until the module attaching to the Network

  NOTE: LAUNCH THIS SKETCH JUST THE FIRST TIME YOU USE THE SHIELD!
  
  Arduino SHIELD: NBduino
  Arduino BOARD: ARDUINO UNO Rev3 (or similar)

  Author: Mariano Iadaresta
  Date: 29/11/2018
*/

#include <SoftwareSerial.h>

#define LED_PIN 13
#define RESET_NBMODULE 7
#define PWRKEY_NBMODULE 8

SoftwareSerial mySerial(10, 11); // RX, TX

// Network settings 
const char* APN = "ep.inetd.gdsp"; //(Vodafone)

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(57600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Let's start!");
  WakeUP_NB_module();
  // set the data rate for the SoftwareSerial port
  mySerial.begin(115200);
  delay(1000);
  mySerial.println("AT+IPR=4800");
  Serial.println("AT+IPR=4800");
  delay (500);
  mySerial.end();
  mySerial.begin(4800);
  delay(500);
 // Setting the APN (Access Point Name) “ep.inetd.gdsp” (GDSP - Global Data Service Platform - VODAFONE)
  mySerial.print("AT*MCGDEFCONT=");
  mySerial.print("\"");
  mySerial.print("IP");
  mySerial.print("\"");
  mySerial.print(",");
  mySerial.print("\"");
  mySerial.print(APN);
  mySerial.println("\"");
  Serial.print("AT*MCGDEFCONT=");
  Serial.print("\"");
  Serial.print("IP");
  Serial.print("\"");
  Serial.print(",");
  Serial.print("\"");
  Serial.print(APN);
  Serial.println("\"");
  delay(500);
  mySerial.print("AT+CGDCONT=1,");
  mySerial.print("\"");
  mySerial.print("IP");
  mySerial.print("\"");
  mySerial.print(",");
  mySerial.print("\"");
  mySerial.print(APN);
  mySerial.print("\"");
  mySerial.print(",");
  mySerial.print("\"");
  mySerial.print("\"");
  mySerial.println(",0,0,0,,,,,,0,,0");
  Serial.print("AT+CGDCONT=1,");
  Serial.print("\"");
  Serial.print("IP");
  Serial.print("\"");
  Serial.print(",");
  Serial.print("\"");
  Serial.print(APN);
  Serial.print("\"");
  Serial.print(",");
  Serial.print("\"");
  Serial.print("\"");
  Serial.println(",0,0,0,,,,,,0,,0");
  delay(500);
  mySerial.println("AT+CEREG=5");
  Serial.println("AT+CEREG=5");
  delay(500);
  mySerial.println("AT+COPS=?");
  Serial.println("AT+COPS=?");
  Serial.println("WAIT UNTIL THE \"NET\" LED STARTS TO FLASH EVERY 3 SECONDS");
}


void WakeUP_NB_module()
{
  digitalWrite(RESET_NBMODULE, LOW);
  digitalWrite(PWRKEY_NBMODULE, HIGH);
  delay(2000);
  digitalWrite(PWRKEY_NBMODULE, LOW);
}
void Sleep_NB_module()
{
  digitalWrite(PWRKEY_NBMODULE, HIGH);
  delay(2000);
  digitalWrite(PWRKEY_NBMODULE, LOW);
}
void loop() { 
  // run over and over
}



