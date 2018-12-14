# NBduino Shield

## Overview


NBduino is the new shield that allows you to send data from your Arduino board directly to the Cloud via the NB-IoT network. You can plug the shield on various Arduino boards (Diecimila, Due, Duemilanove, Uno, Leonardo, Mega2560, Yun) or it can be used standalone with a LiPo battery.

The key features are:
- **Plug & Play** (Arduino compatible)
- Power modes (USB or LiPo battery)
- **Low Power** (5µA)
- **Long range** (up to 7Km)
- Cloud Storage (MQTT Broker ready)

## Examples

 Inside the *Code/examples* directory there are three Arduino sketches useful to do initial test of the board and make a connection to a MQTT broker to publish sensor's data.

 

 1. **NBduino_NetSettings.ino**: used only the first time to set network settings and register to the network;
 2. **NBduino_MQTT_HELLO_WORLD.ino**:  used to publish, over MQTT protocol, "Hello World!" as "payload" and the SHIELD's IMEI as "Topic";
 3.  **NBduino_SI7006.ino**: a simple sketch used to publish humidity and temperature data from a [SI7006-A20-IM](https://www.silabs.com/documents/public/data-sheets/Si7006-A20.pdf) sensor to MQTT broker.
