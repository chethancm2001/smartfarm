#include<DHT.h>
#include <ESP8266WiFi.h>

#define sensor A0
#define dht_pin D1
#define dht_type DHT11

String apiKey = "BY7P6J5KJIKXOCKP";     //  Enter your Write API key from ThingSpeak

const char *ssid =  "Galaxy A70";     // replace with your wifi ssid and wpa2 key
const char *pass =  "ammanana";
const char *server = "api.thingspeak.com";

DHT farm(dht_pin,dht_type);
WiFiClient client;

void setup() {
  Serial.begin(9600);
  farm.begin();
  pinMode(sensor,OUTPUT);
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) 
     {
            delay(500);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("WiFi connected");
}


void loop() {
  int moisture_percentage;
  delay(2000);
  float t=farm.readTemperature();
  int h=farm.readHumidity();
  moisture_percentage = ( 100.00 - ( (analogRead(sensor)/1023.00) * 100.00 ) );
  
  Serial.println(t);
  Serial.print(h);
  Serial.print(moisture_percentage);

if (isnan(h) || isnan(t)) 
                 {
                     Serial.println("Failed to read from DHT sensor!");
                      return;
                 }
 
                         if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
                      {  
                            
                             String postStr = apiKey;
                             postStr +="&field1=";
                             postStr += String(t);
                             postStr +="&field2=";
                             postStr += String(h);
                             postStr +="&field3=";
                             postStr += String(moisture_percentage);
                             postStr += "\r\n\r\n";
 
                             client.print("POST /update HTTP/1.1\n");
                             client.print("Host: api.thingspeak.com\n");
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(postStr.length());
                             client.print("\n\n");
                             client.print(postStr);
 
                             Serial.print("Temperature: ");
                             Serial.print(t);
                             Serial.print(" degrees Celcius, Humidity: ");
                             Serial.print(h);
                             Serial.println("%. Send to Thingspeak.");
                             Serial.print("soil moisture");
                             Serial.println(moisture_percentage);
                        }
          client.stop();
 
          Serial.println("Waiting...");
  
  // thingspeak needs minimum 15 sec delay between updates
  delay(1000);
  
 

}
