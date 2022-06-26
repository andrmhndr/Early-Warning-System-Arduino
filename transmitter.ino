#include <SPI.h>
#include <LoRa.h>
#include "DHT.h" 
#include <Arduino.h>
#include <stdio.h>

 #define Sensor_Out_Pin A5
#define smokeA0 A4
#define DHTPIN A0     
#define DHTTYPE DHT11   
 
DHT dht(DHTPIN, DHTTYPE);
int hum;
float temp; //Stores temperature value
long getar;
int smoke;
 
void setup() {    
  Serial.begin(9600);
  dht.begin();
  while (!Serial);
  Serial.println("LoRa Sender");
 
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}
 
void loop() 
{
  temp = dht.readTemperature();
  hum = dht.readHumidity();
  getar = pulseIn(Sensor_Out_Pin, HIGH);
  smoke = analogRead(smokeA0);
  
  Serial.println("");
  Serial.println("Sending packet: ");
  Serial.println("");
 
  // send packet
  LoRa.beginPacket();
  LoRa.println(""); 
  LoRa.print(hum);
  LoRa.print("!");
  LoRa.print(temp);
  LoRa.print("@");
  LoRa.print(getar);
  LoRa.print("#");
  LoRa.println(smoke);
  
  Serial.print("Humidity: ");
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
  
 
  LoRa.endPacket();
  delay(1000);
}
