#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHTesp.h>
#include "SSD1306.h"
#include <Wire.h>
//GPIO Pin mapping 부분, nodemcu와 아두이노간 pin이 달라서 초기 설정 필요
#define D0 16 // LED
#define D1 5
#define D2 4
#define D3 0 // DHT22
#define D4 2
#define D5 14
#define D6 12
#define D7 13
#define D8 15
//int LED_PIN = D0;
int LED_PIN = D5;
#define RELAY1_PIN D4
int DHT_PIN = D3;
int CDS_PIN = A0;
// Sensors and Actuators
#define DHTTYPE DHT22
#define LED_ON HIGH
#define LED_OFF LOW
#define RELAY_ON LOW
#define RELAY_OFF HIGH
DHTesp dht;
int led_state, usbled_state;
char str_temperature[40], str_humidity[40], str_light_intensity[40];
SSD1306 display(0x3C, D2, D1); // // ADDRESS, SDA, SCL for 128x32
// Wifi
const char *wifi_ssid = "Jacob's"; // 사용하는 공유기 이름
const char *wifi_password = "12341234"; // 공유기 password 입력

// MQTT
#define mqtt_broker "203.252.106.154" //MQTT broker address(자신의 Raspberry Pi3 IP), 노트북을 통한 모바일 핫스팟에서 IP check
#define mqtt_user "iot"
#define mqtt_pwd "csee1414"
#define TOPIC_GREETINGS "Hello from nodeMCU at NTH405"
String mqtt_payload;
// 개별 nodeMCU 설정시 nth405를 학번으로 변경
//Publish를 위한 Topic
const char *mqtt_nodeName = "nodeMCU.21901019"; 
const char *pub_dht = "iot/21901019/dht22"; 
const char *pub_cds = "iot/21901019/cds"; 
//Subscribe Topic
const char *sub_topic = "iot/21901019"; 
WiFiClient wifiClient; // WIFI client
//DHT dht(DHT_PIN, DHTTYPE); // 온습도 센서 instance 선언
float temperature, humidity;
int lightValue;
char pub_data[80]; //publishing 할때 사용
void callback(char* topic, byte* payload, unsigned int length);
PubSubClient mqttc(mqtt_broker, 1883, callback, wifiClient); // MQTT client

void callback(char *topic, byte *payload, unsigned int length){ //Web으로 부터 수신한 Message에 따른동작 제어 함수
  //
  digitalWrite(LED_PIN, LED_ON);
  Serial.print("asfd");

} /* end of callback() for MQTT */

void setup() {
  dht.setup(DHT_PIN, DHTesp::DHT22);
  Serial.begin(9600);
  delay(10);
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_24); // 10 16 24
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  
  pinMode(LED_PIN, OUTPUT); // Initialize the LED_BUILTIN pin as an output
  digitalWrite(LED_PIN, LED_ON); //LED on
  pinMode(RELAY1_PIN, OUTPUT);
  digitalWrite(RELAY1_PIN, RELAY_OFF);
  pinMode(CDS_PIN,INPUT);
  InitWiFi();
  //Connection to MQTT broker
  if (mqttc.connect(mqtt_nodeName, mqtt_user, mqtt_pwd)){
    mqttc.publish(pub_dht, TOPIC_GREETINGS); // testing message of publish
    mqttc.subscribe(sub_topic);//Subscribe 할 Topic
    
    humidity = dht.getHumidity();  
    temperature = dht.getTemperature();
    lightValue = analogRead(CDS_PIN); 
    sprintf(str_temperature, "Temperature : %3.1f", temperature);
    sprintf(str_humidity, "Humidity : %3.1f", humidity);
    sprintf(str_light_intensity, "Light Value : %d", lightValue);
    mqttc.publish("iot/21901019",  str_temperature); // You can activate the retain flag by setting the third parameter to true
    mqttc.publish("iot/21901019", str_humidity);
    mqttc.publish("iot/21901019", str_light_intensity);

    sprintf(str_temperature, "%3.1f", temperature);
    sprintf(str_humidity, "%3.1f", humidity);
    sprintf(str_light_intensity, "%d", lightValue);
    
    display.clear();
    display.drawString(0, 0, "T: " + String(str_temperature) + "C"); 
    display.drawString(0, 20, "H: " + String(str_humidity) + "%\t");
    display.drawString(0, 40, "B: " + String(str_light_intensity));
    display.display();
  }
  ESP.deepSleep(30e6);
}


void loop() { 
  digitalWrite(LED_PIN, LED_ON);
  Serial.print("Is't sleep");
//  if(!wifiClient.connected()){
//    Serial.print("recon");
//    reconnect();
//  }
//  delay(500); // delay 
//  mqttc.loop();
//  ESP.deepSleep(30e6);
} /* end of loop() */
  
void InitWiFi(){
  Serial.println();
  Serial.print(".");
  // attempt to connect to WiFi network
  WiFi.begin(wifi_ssid, wifi_password);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  // 접속이 되면 출력
  Serial.println();
  Serial.print("Connected to AP: ");
  //접속 정보를 출력
  Serial.println(WiFi.localIP());
//  Serial.println(WiFi.status());
}


void reconnect(){
  int status;
  //네트워크 접속
  while (!wifiClient.connected()){
    status = WiFi.status();
    Serial.println(wifiClient.connected());
    if( status != WL_CONNECTED) {
      WiFi.begin(wifi_ssid, wifi_password);
      while(WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println(".");
      }
      Serial.println("Connected to AP again");
    }
    delay(5000); 
  }//end while
}//Network 접속 확인 함







  
