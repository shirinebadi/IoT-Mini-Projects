#include <ESP8266WiFi.h>

const char* ssid     = "Rahanet-Ebadi";     
const char* password = "#428@578"; 

void setup() {
  Serial.begin(9600);

  WiFi.mode(WIFI_STA);
  
  delay(2000);

  Serial.println("Started Scanning");

  delay(5000);
  
  WiFi.disconnect(); 

  connectWifi();
}


void loop() {
//  int n = WiFi.scanNetworks();
//  
//  if (n == 0)
//    Serial.println("No Networks Found");
//  else
//  {
//    for (int i = 0; i < n; ++i)
//    {
//   
//      Serial.print(i + 1); 
//      Serial.print(": ");
//      Serial.print(WiFi.SSID(i)); 
//      Serial.println(" ");
//    }
//  }
//
//  delay(5000);
}

void connectWifi(){
  WiFi.begin(ssid, password);   
           
  Serial.print("Connecting to ");
  Serial.println(ssid);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Waiting for Connection");
  }

  Serial.print("Connected to ");  
  Serial.println(ssid);
  
  Serial.print("IP Address:  ");
  Serial.println(WiFi.localIP()); 
}
