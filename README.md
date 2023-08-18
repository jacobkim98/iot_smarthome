# iot_smarthome

Introduction:
This document aims to utilize Deep-Sleep mode to minimize energy consumption for integrating ESP8266 into an IoT system. During program execution, temperature, humidity, and light data are published to a Raspberry Pi using the MQTT protocol.

Usage:
1. Start the Raspberry Pi and prepare to receive MQTT messages by using the following command:
      mosquitto_sub -h 203.252.106.154 -t "iot/21901019" -u "iot" -P "csee1414"
2. As the 'iot_lab1_sensor_node_final.ino' file contains Wi-Fi settings, adjust the Wi-Fi name and password according to your environment.
3. Power up the ESP8266 module. The file to be executed is 'iot_lab1_sensor_node_final.ino'.
4. Over time, you will observe temperature, humidity, and light data being received and displayed on the Raspberry Pi.
5. By employing this approach, you can efficiently conserve energy while reliably collecting essential data in your IoT system.

libraries:
- DHT22 - DHT sensor library for ESPx
- OLED – adafruit_SSD1306, adafruit_gfx

Device:
- LED
- DHT22
- Relay(USBLED)
- CDS
- ESP8266

