#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid     = "NodeMCU";     
const char* password = "12345678"; 
 
void setup() {
 
  Serial.begin(115200);

  delay(4000);

  IPAddress local_ip(192,168,1,5);
  IPAddress gateway(192,168,1,5);
  IPAddress subnet(255,255,255,0);
  WiFi.softAPConfig(local_ip, gateway, subnet);
 
  Serial.print("Server IP address: ");
  Serial.println(WiFi.softAPIP());

  delay(6000);
 
  WiFi.softAP(ssid, password);
 
}
 
void loop() {
 
}
