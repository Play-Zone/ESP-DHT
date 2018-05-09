/*
 *  Sample-Sketch for the DHT11-ESP-Module
 *  (C) 2018 PLAY-ZONE GmbH www.play-zone.ch
 *  
 *  Adapted from the sketch from https://www.elec-cafe.com/esp8266-temperature-humidity-webserver-with-a-dht11-sensor/
 * 
 * Updated with the DTHesp-Library: https://desire.giesecke.tk/index.php/2018/01/30/esp32-dht11/
 * (Install thru Library-Manager, search for "DHT ESPX")
 * 
 */

#include <ESP8266WiFi.h>
#include "DHTesp.h"

const char* ssid     = "ssid"; // Your ssid
const char* password = "password"; // Your Password

int pin = 2;

WiFiServer server(80);
DHTesp dht;

double Fahrenheit(double celsius) {
  return ((double)(9 / 5) * celsius) + 32;
}

double Kelvin(double celsius) {
  return celsius + 273.15;
}

void setup() {
  dht.setup(pin);
  Serial.begin(115200);
  delay(10);
  Serial.println();

  //  Connect to WiFi network
  WiFi.mode(WIFI_STA);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop() {
  delay(1000);
  
  float temp, humi;
  
  humi = dht.getHumidity();
  temp = dht.getTemperature();

  if (!isnan(humi) && !isnan(temp))
  {
    Serial.print("temperature:");
    Serial.print(temp);
    Serial.print(" humidity:");
    Serial.print(humi);
    Serial.println();
  }
  else
  {
    Serial.println();
    Serial.print("Error Reading DHT");
  }
  
  WiFiClient client = server.available();
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");  // the connection will be closed after completion of the response
  client.println("Refresh: 5");  // refresh the page automatically every 5 sec
  client.println();
  client.println("<!DOCTYPE html>");
  client.println("<html xmlns='http://www.w3.org/1999/xhtml'>");
  client.println("<head>\n<meta charset='UTF-8'>");
  client.println("<title>ESP8266 Temperature & Humidity DHT11 Sensor</title>");
  client.println("</head>\n<body>");
  client.println("<H2>ESP8266 & DHT11 Sensor</H2>");
  client.println("<H3>Humidity / Temperature</H3>");
  client.println("<pre>");
  client.print("Humidity (%)      : ");
  client.println((float)humi, 2);
  client.print("Temperature (°C)  : ");
  client.println((float)temp, 2);
  client.print("Temperature (°F)  : ");
  client.println(Fahrenheit(temp), 2);
  client.print("Temperature (°K)  : ");
  client.println(Kelvin(temp), 2);
  client.println("</pre>");
  client.println("<H3>www.play-zone.ch</H3>");
  client.print("</body>\n</html>");
}
