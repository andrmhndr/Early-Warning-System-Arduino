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
String stat;
 
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

  //normal
  if(temp <= 35 && hum > 15 && getar <= 10 && smoke <= 50){
    stat = "Normal";
  }
  //waspada
  else if(temp > 35 && temp <= 38 && hum > 10 && hum <= 14 && getar > 10 && smoke > 50 && smoke <= 500 ){
    stat = "Waspada";
  }
  //siaga
  else if(temp > 38 && temp <= 40 && hum > 5 && hum <= 9 && getar > 20 && smoke > 500 && smoke <= 1000){
    stat = "Siaga"; 
  }
  //awas
  else if(temp > 40 && hum > 4 && hum <= 0 && getar > 50 && smoke > 1000){
    stat = "Awas";
  }
  //random
  else {
    stat = "Normal";
  }
  
  Serial.println("");
  Serial.println("Sending packet: ");
  Serial.println("");
 
  // send packet
  LoRa.beginPacket();
  LoRa.print(hum);
  LoRa.print("!");
  LoRa.print(temp);
  LoRa.print("@");
  LoRa.print(getar);
  LoRa.print("#");
  LoRa.print(smoke);
  LoRa.print("$");
  LoRa.print(stat);
  
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
  Serial.print("Status:");
  Serial.println(stat);
 
  LoRa.endPacket();
  delay(1000);
}
