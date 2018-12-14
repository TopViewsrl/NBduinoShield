/*
  Plug your NBduino SHIELD on the Arduino board,run this sketch, after loading is complete open the IDE serial monitor
  publish, over MQTT protocol, "Hello World!" as "payload" and the SHIELD's IMEI as "Topic"

  Arduino SHIELD: NBduino
  Arduino BOARD: ARDUINO UNO Rev3 (or similar)

  Author: Mariano Iadaresta
  Date: 27/11/2018
*/

#include <SoftwareSerial.h>
#include "Timer.h"

#define LED_PIN 13
#define RESET_NBMODULE 7
#define PWRKEY_NBMODULE 8

// Cloud authentication settings
const char* mqttServer = "xxx.xxx.xxx.xxx"; // MQTT Server IP address 
const int mqttPort = 18224; // Port
const char* mqttUser = "test"; // Username
const char* mqttPassword = "test"; // Password

Timer t;

SoftwareSerial mySerial(10, 11); // RX, TX

int incomingByte = 0;    // for incoming serial data

char inChar;
char aMessage[200];                               // to hold the message received
char cha;                                         // for reading char one at a time
byte messageSize;                                 // to hold the received message sixe

String Topic;
String Payload = "48656c6c6f20576f726c6421"; //"Hello World!" in HEX format
unsigned int PayloadLength = Payload.length();     // previous length of the String

boolean req_reg = true;  // whether the module is attached to the network

unsigned long lastTime = 0;
unsigned long TimeToConn = 120000;    // time for attaching to the network, 120 seconds
unsigned long Pub_Interval = 20000; // publication interval in ms, 20 seconds

String exString;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(57600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Let's start!");
  delay(300);
  WakeUP_NB_module();
  // set the data rate for the SoftwareSerial port
  mySerial.begin(115200);
  delay(1000);
  mySerial.println("AT+IPR=4800");
  Serial.println("AT+IPR=4800");
  delay (500);
  mySerial.end();
  mySerial.begin(4800);
  delay (500);
  mySerial.println("ATE0");
  Serial.println("ATE0");
  mySerial.println("AT+CEREG=5"); //restituisce i parametri della cella LTE
  Serial.println("AT+CEREG=5");
  Serial.println("NETWORK ATTACH REQUEST");
  Serial.print(".");
  delay (500);
  Serial.print(".");
  delay (500);
  Serial.println(".");
  delay (500);
  mySerial.flush();
  req_reg = true;
  lastTime = millis();
  while ( req_reg )
  {
    if (millis() - lastTime < TimeToConn) {
      // get incoming byte:
      inChar = mySerial.read();
      if ( inChar == '+') {
        req_reg = false;
        Serial.println("connected to the mobile Network!");
        Serial.println(millis() - lastTime);
        // modulo agganciato procedi con la pubblicazione
        break;
      }
    }
    else if ( millis() - lastTime > TimeToConn) {
      Serial.println("failed to connect!");
      break;
    }
  }
  lastTime = 0;
  int tickEvent = t.every(Pub_Interval, Pub);
}

void Pub()
{
  digitalWrite( LED_PIN , HIGH );
  delay(2000);
  mySerial.print("AT+CMQNEW=\"");
  mySerial.print(mqttServer);
  mySerial.print("\",""\"");
  mySerial.print(mqttPort);
  mySerial.println("\",2400,100");
  Serial.print("AT+CMQNEW=\"");
  Serial.print(mqttServer);
  Serial.print("\",""\"");
  Serial.print(mqttPort);
  Serial.println("\",2400,100");
  delay(2000);
  mySerial.print("AT+CMQCON=0,3,");
  mySerial.print("\"");
  mySerial.print(Topic);
  mySerial.print("\"");
  mySerial.print(",1200,0,0,");
  mySerial.print("\"""");
  mySerial.print(mqttUser);
  mySerial.print("\",""\"");
  mySerial.print(mqttPassword);
  mySerial.println("\"");
  Serial.print("AT+CMQCON=0,3,");
  Serial.print("\"");
  Serial.print(Topic);
  Serial.print("\"");
  Serial.print(",1200,0,0,");
  Serial.print("\"""");
  Serial.print(mqttUser);
  Serial.print("\",""\"");
  Serial.print(mqttPassword);
  Serial.println("\"");
  delay(2000);
  mySerial.print("AT+CMQPUB=0,\"");
  mySerial.print("NBduino/");
  mySerial.print(Topic);
  mySerial.print("\",1,0,0,");
  mySerial.print(PayloadLength);
  mySerial.print(",");
  mySerial.print("\"");
  mySerial.print(Payload);
  mySerial.println("\"");
  Serial.print("AT+CMQPUB=0,\"");
  Serial.print("NBduino/");
  Serial.print(Topic);
  Serial.print("\",1,0,0,");
  Serial.print(PayloadLength);
  Serial.print(",");
  Serial.print("\"");
  Serial.print(Payload);
  Serial.println("\"");
  delay(2000);
  mySerial.println("AT+CMQDISCON=0");
  Serial.println("AT+CMQDISCON=0");
  delay(2000);
  digitalWrite(LED_PIN , LOW);   //LED OFF
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

  t.update();

  // request for IMEI module (used as "TOPIC")
  mySerial.println("AT+GSN");
  mySerial.flush();
  if (mySerial.available()) {
    for (int c = 0; c <= 99; c++) aMessage[c] = 0;    // clear aMessage in prep for new message
    messageSize = 0;                                  // set message size to zero

    while (mySerial.available()) {            // loop through while data is available
      cha = mySerial.read();                  // get character
      aMessage[messageSize] = cha;            // append to aMessage
      messageSize++;                          // bump message size
      delay(20);                              // just to slow the reads down a bit
    } // while

    aMessage[messageSize] = '\0';             // set last character to a null

    String exString = aMessage;
    exString.remove(0, 2);
    exString.remove(15);
    Topic = exString;
    Serial.print("IMEI module:");
    Serial.println(exString);
  }

  delay(1000);
}



