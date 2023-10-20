#include "DHT.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>
#include <Ethernet.h>
#define ONE_WIRE_BUS 8 //assigning pin #8 to ds18b20
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
#define DHTPIN 2 //assigning pin 2 to dht sensor
#define DHTTYPE DHT11 //sensor type dht 11
DHT dht(DHTPIN,DHTTYPE);

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
uint8_t ds1[8] = {0x28, 0x00, 0x32, 0x95, 0xF0, 0x01, 0x3C, 0x2D};
uint8_t ds2[8] = {0x28, 0x3F, 0xC9, 0x76, 0xE0, 0x01, 0x3C, 0xB9};

float wilgotnoscDHT;
float temperaturaDHT;
float ds1t,ds2t;
float sr;

const String key = "YourKey";
char server[]= "api.thingspeak.com";
////the IP address is dependent on your network
IPAddress ip("put your adress"); 
EthernetClient client;




void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
sensors.begin();
dht.begin();

if (Ethernet.begin(mac) == 0) {
  Serial.println("Error during configuration");
  Ethernet.begin(mac, ip);
  }
  delay(1000);
}


void loop() {
delay(1000);
sensors.requestTemperatures();
Serial.print("Sensor 1: ");
ds1t=wyswietlTemp(ds1); 
Serial.print("Sensor 2: ");
ds2t=wyswietlTemp(ds2);


wilgotnoscDHT = dht.readHumidity();
temperaturaDHT = dht.readTemperature(); 
sr = (temperaturaDHT+ds1t+ds2t)/3; //Average
if (isnan(wilgotnoscDHT) || isnan(temperaturaDHT))
  {
    Serial.println("Measurement error from dht sensor");
  }

  
Serial.println();
Serial.print("DHT11 humidity: ");
Serial.print(wilgotnoscDHT);
Serial.println();
Serial.print("Temperature DHT11: ");
Serial.print(temperaturaDHT);
Serial.println();
Serial.print("Average temperature: ");
Serial.print(sr);
Serial.println();
delay(10000);
Serial.println();

thing_speak(); 
  delay(60000);
}
float wyswietlTemp(DeviceAddress deviceAddress)
 {
  float tempC = sensors.getTempC(deviceAddress);
  Serial.print(tempC);
  Serial.print("°C  |  ");
  return tempC;
 }
 
void thing_speak()
{
if(client.connect(server, 80))
  {
  Serial.println("connected ");
  client.print(String("GET /update?api_key=")+ key + ("&field1=")+
  temperaturaDHT + ("&field2=") + wilgotnoscDHT 
  + ("&field3=") + ds1t + ("&field4=") + ds2t + ("&field5=") + sr + ("\n"));
   
  client.print("HTTP/1.1");
  client.println();
  client.println("Host: api.thingspeak.com");
  client.println("Connection: closed");
  client.println();
  }
else
  {
  Serial.println("Connection error");
  }
  
}
