#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

long duration;
int distance;

ESP8266WebServer server(80);

const char* ssid     = "NodeMCU";     
const char* password = "12345678"; 

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    .slider { -webkit-appearance: none; margin: 14px; width: 360px; height: 25px; background: #FFD69C;
      outline: none; -webkit-transition: .2s; transition: opacity .2s;}
    .slider::-webkit-slider-thumb {-webkit-appearance: none; appearance: none; width: 35px; height: 35px; background: #003249; cursor: pointer;}
    .slider::-moz-range-thumb { width: 35px; height: 35px; background: #003249; cursor: pointer; } 
  </style>
</head>
<body>
  <h2>Shirin's Servo Controller</h2>
  <p><span id="textSliderValue">%SLIDERVALUE%</span></p>
  <p><input type="range" id="pwmSlider" min="0" max="30" value="%SLIDERVALUE%" step="1" class="slider"></p>
<script>

setInterval(function() 
{
  getData();
}, 1000); 
function getData() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("pwmSlider").value =
      this.responseText;

      document.getElementById("textSliderValue").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "adcread", true);
  xhttp.send();
}
</script>
</body>
</html>
)rawliteral";

void setup() {

  
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D0, OUTPUT); 
  pinMode(D1, INPUT); 

  digitalWrite(D2, HIGH);
  digitalWrite(D3, LOW);
  
  delay(5000);

  IPAddress local_ip(192,168,1,5);
  IPAddress gateway(192,168,1,5);
  IPAddress subnet(255,255,255,0);
  WiFi.softAPConfig(local_ip, gateway, subnet);
 
  Serial.begin(115200);

  WiFi.softAP(ssid,password);

  delay(5000);

  Serial.print("Connected to ");
  Serial.println(ssid);
  
  server.on("/", handleRoot);
  server.on("/adcread", setDis);

   server.begin();
  
  Serial.println("Web server started!");

}

void loop() {

  server.handleClient();
}

void setDis(){
    digitalWrite(D0, LOW);
    delayMicroseconds(2);
    digitalWrite(D0, HIGH);
    delayMicroseconds(10);
    digitalWrite(D0, LOW);

    duration = pulseIn(D1, HIGH);
    
    distance= duration*0.034/2;

    if (distance < 5){
       digitalWrite(D2, LOW);
       digitalWrite(D3, HIGH);
    }

    else{
       digitalWrite(D2, HIGH);
       digitalWrite(D3, LOW);
    }
    
    Serial.println(distance);

    String sdistance = String(distance);
    server.send(200, "text/plane", sdistance);
 
}

void handleRoot() 
{
 
 server.send(200, "text/html", index_html);
}
