#include <DHT.h>
#include <DHT_U.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SDS011.h>
#include <SoftwareSerial.h>


#define SDS_RX D1
#define SDS_TX D2

#define DHTPIN D4 
#define DHTTYPE DHT22   

float p10,p25;
int error;

const char* ssid     = "SSID";
const char* password = "secret";

DHT_Unified dht(DHTPIN, DHTTYPE);
ESP8266WebServer server(80);
SDS011 sds;

void setup() {
  Serial.begin(9600);
  sds.begin(SDS_RX,SDS_TX);
  dht.begin();
  connectToWiFi();
}

void loop() {
  server.handleClient();
}

void connectToWiFi(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
 
  Serial.print("Connecting to ");
  Serial.println(ssid); 

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.begin();
    Serial.println("HTTP server started");

}

void handleRoot() {
  String temp;
  String humidity;
  error = sds.read(&p25,&p10);
  if (! error) {
     sensors_event_t event;  
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println("Error reading temperature!");
  }
  else {
    Serial.print("Temperature: ");
    Serial.print(event.temperature);
    Serial.println(" *C");
    temp = String(event.temperature) + " *C";
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println("Error reading humidity!");
  }
  else {
    Serial.print("Humidity: ");
    Serial.print(event.relative_humidity);
    Serial.println("%");
    humidity = String(event.relative_humidity) + "%";
  }
    server.send(200, "text/plain", "PM2.5: " + String(p25) + " | PM10: " +  String(p10) + " | Temperature: " + temp + " | Humidity: " + humidity);  
  }
}

