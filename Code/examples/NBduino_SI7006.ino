/*
  Plug your NBduino SHIELD on the Arduino board, connect the sensor and run this sketch
  
  Arduino SHIELD: NBduino
  Arduino BOARD: ARDUINO UNO Rev3 (or similar)
  SENSOR: SI7006-A20-IM, T°/H% 

  Arduino PIN | SI7006-A20-IM PIN
  3.3V        | VCC
  GND         | GND
  A5          | SCL
  A4          | SDA

  Author: Mariano Iadaresta
  Date: 22/11/2018
*/

#include <SoftwareSerial.h>
#include "Timer.h"
#include <Wire.h>

// SI7006-A20 I2C address is 0x40(64)
#define Addr 0x40

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

String Topic_IMEI;

boolean req_reg = true;  // whether the module is attached to the network

unsigned long lastTime = 0;  
unsigned long TimeToConn = 60000;    // time for attaching to the network

float humidity = 0;
float temp  = 0;
float ctemp = 0;
float ftemp = 0;

String exString;

void setup() {
  // Initialise I2C communication as MASTER
  Wire.begin();
  // Open serial communications and wait for port to open:
  Serial.begin(57600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Let's start!");
  // Start I2C transmission
  Wire.beginTransmission(Addr);
  // Stop I2C transmission
  Wire.endTransmission();
  delay(300);
  WakeUP_NB_module();
  // set the data rate for the SoftwareSerial port
  mySerial.begin(115200);
  mySerial.println("ATE0");
  req_reg = true;
  delay(1000);
  mySerial.println("AT+IPR=115200");
  Serial.println("AT+IPR=115200");
  delay (500);
  mySerial.println("AT+CEREG=5"); //restituisce i parametri della cella LTE
  Serial.println("AT+CEREG=5");
  delay (500);
  mySerial.flush();
  lastTime = millis();
  while ( req_reg )
  {
    if (millis() - lastTime < TimeToConn) {
      // get incoming byte:
      inChar = mySerial.read();
      if ( inChar == '+') {
        req_reg = false;
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
  int tickEvent = t.every(20000, Pub);
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
  mySerial.print(Topic_IMEI);
  mySerial.print("\"");
  mySerial.print(",1200,0,0,");
  mySerial.print("\"""");
  mySerial.print(mqttUser);
  mySerial.print("\",""\"");
  mySerial.print(mqttPassword);
  mySerial.println("\"");
  Serial.print("AT+CMQCON=0,3,");
  Serial.print("\"");
  Serial.print(Topic_IMEI);
  Serial.print("\"");
  Serial.print(",1200,0,0,");
  Serial.print("\"""");
  Serial.print(mqttUser);
  Serial.print("\",""\"");
  Serial.print(mqttPassword);
  Serial.println("\"");
  delay(2000);
  String Hum = String(humidity);
  String temp = String(ctemp);
  mySerial.print("AT+CMQPUB=0,\"");
  mySerial.print(Topic_IMEI);
  mySerial.print("/");
  mySerial.print("H");
  mySerial.print("\",1,0,0,4,");
  mySerial.print("\"3");
  mySerial.print(Hum[0]);
  mySerial.print("3");
  mySerial.print(Hum[1]);
  mySerial.println("\"");
  Serial.print("AT+CMQPUB=0,\"");
  Serial.print(Topic_IMEI);
  Serial.print("/");
  Serial.print("H%");
  Serial.print("\",1,0,0,4,");
  Serial.print("\"3");
  Serial.print(Hum[0]);
  Serial.print("3");
  Serial.print(Hum[1]);
  Serial.println("\"");
  mySerial.print("AT+CMQPUB=0,\"");
  mySerial.print(Topic_IMEI);
  mySerial.print("/");
  mySerial.print("T°");
  mySerial.print("\",1,0,0,4,");
  mySerial.print("\"3");
  mySerial.print(temp[0]);
  mySerial.print("3");
  mySerial.print(temp[1]);
  mySerial.println("\"");
  Serial.print("AT+CMQPUB=0,\"");
  Serial.print(Topic_IMEI);
  Serial.print("/");
  Serial.print("T");
  Serial.print(char(176));
  Serial.print("\",1,0,0,4,");
  Serial.print("\"3");
  Serial.print(temp[0]);
  Serial.print("3");
  Serial.print(temp[1]);
  Serial.println("\"");
  delay(2000);
  mySerial.println("AT+CMQDISCON=0");
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
void loop() { // run over and over
  t.update();
  uint8_t data[2] = {0};

  // Start I2C transmission
  Wire.beginTransmission(Addr);
  // Send humidity measurement command, NO HOLD MASTER
  Wire.write(0xF5);
  // Stop I2C transmission
  Wire.endTransmission();
  delay(500);

  // Request 2 bytes of data
  Wire.requestFrom(Addr, 2);

  // Read 2 bytes of data
  // humidity msb, humidity lsb
  if (Wire.available() == 2)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
  }

  // Convert the data
  humidity  = ((data[0] * 256.0) + data[1]);
  humidity = ((125 * humidity) / 65536.0) - 6;
  
  // Start I2C transmission
  Wire.beginTransmission(Addr);
  // Send temperature measurement command, NO HOLD MASTER
  Wire.write(0xF3);
  // Stop I2C transmission
  Wire.endTransmission();
  delay(500);

  // Request 2 bytes of data
  Wire.requestFrom(Addr, 2);

  // Read 2 bytes of data
  // temp msb, temp lsb
  
  if (Wire.available() == 2)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
  }
  // Convert the data
  temp  = ((data[0] * 256.0) + data[1]);
  ctemp = ((175.72 * temp) / 65536.0) - 46.85;
  ftemp = ctemp * 1.8 + 32;
  
  // Output data to serial monitor
  Serial.print("Relative humidity : ");
  Serial.print(humidity);
  Serial.println(" % RH");
  Serial.print("Temperature in Celsius : ");
  Serial.print(ctemp);
  Serial.println(" C");
  Serial.print("Temperature in Fahrenheit : ");
  Serial.print(ftemp);
  Serial.println(" F");

  // request for IMEI module (used as "TOPIC")
  mySerial.println("AT+GSN");

  if (mySerial.available()) {
    for (int c = 0; c <= 99; c++) aMessage[c] = 0;    // clear aMessage in prep for new message
    messageSize = 0;                                  // set message size to zero

    while (mySerial.available()) {                    // loop through while data is available
      cha = mySerial.read();                  // get character
      aMessage[messageSize] = cha;            // append to aMessage
      messageSize++;                          // bump message size
      delay(20);                              // just to slow the reads down a bit
    } // while

    aMessage[messageSize] = '\0';             // set last character to a null

    String exString = aMessage;
    exString.remove(0, 2);
    exString.remove(15);
    Topic_IMEI = exString;
    Serial.print("IMEI module:");
    Serial.println(exString);              
  }
  
  delay(1000);
}



