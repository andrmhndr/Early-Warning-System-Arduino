#include <SPI.h>
#include <LoRa.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define ss 15
#define rst 16
#define dio0 2

// Set these to run example.
#define FIREBASE_HOST "early-warning-46262-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH "Y1yArdLWM1s8l6mQnvu2yC03Zs4kzJVq3x2lCQF8"
#define WIFI_SSID "Pojok TE"
#define WIFI_PASSWORD "rahasiabentar"

void setup() 
{
  Serial.begin(9600);
  
  while (!Serial);
  Serial.println("LoRa Receiver");
  
  LoRa.setPins(ss, rst, dio0);

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  
}
 
void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) 
  {
    // received a paket
    Serial.println("...................................");
    Serial.println("Received packet: ");

    String hum;
    String temp;
    String getar;
    String smoke;
    
    String data;
    // read packet
    while (LoRa.available()) {
      char incoming = (char)LoRa.read();
      if (incoming == 'c')
      {
        Serial.print("Error");
      }
      else
      {
        data.concat(incoming);
      }
    }
    int humIndex = data.lastIndexOf('!');
    int tempIndex = data.lastIndexOf('@');
    int getarIndex = data.lastIndexOf('#');
    int smokeIndex = data.length();

    hum = data.substring(0,humIndex);
    temp = data.substring(humIndex+1,tempIndex);
    getar = data.substring(tempIndex+1, getarIndex);
    smoke = data.substring(getarIndex+1, smokeIndex);

    Firebase.setString("data/humidity",hum);
    Firebase.setString("data/temperature",temp);
    Firebase.setString("data/getar",getar);
    Firebase.setString("data/smoke",smoke);

    Serial.print("Humidity:" );
    Serial.print(hum);
    Serial.println("%");
    Serial.print("Temperature:");
    Serial.print(temp);
    Serial.println("C");
    Serial.print("Getaran:");
    Serial.print(getar);
    Serial.println(""); 
    Serial.print("Smoke:");
    Serial.print(smoke);
    Serial.println("");    
  }
}
