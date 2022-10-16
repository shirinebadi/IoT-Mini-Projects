#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESPAsyncWebServer.h>
#include <Servo.h>
#include <ESPAsyncTCP.h>

Servo servo;

String sliderValue = "0";

const char* PARAM_INPUT = "value";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

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
  <p><input type="range" onchange="updateSliderPWM(this)" id="pwmSlider" min="0" max="90" value="%SLIDERVALUE%" step="1" class="slider"></p>
<script>
function updateSliderPWM(element) {
  var sliderValue = document.getElementById("pwmSlider").value;
  document.getElementById("textSliderValue").innerHTML = sliderValue;
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/slider?value="+sliderValue, true);
  xhr.send();
}
</script>
</body>
</html>
)rawliteral";

void setup() {

  servo.attach(2); //D4

  servo.write(0);

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
  
   
   server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  server.on("/slider", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    // GET input1 value on <ESP_IP>/slider?value=<inputMessage>
    if (request->hasParam(PARAM_INPUT)) {
      inputMessage = request->getParam(PARAM_INPUT)->value();
      sliderValue = inputMessage;
      int servoVal = sliderValue.toInt() + 30;
      servo.write(servoVal);
      delay(1000);
    }
    request->send(200, "text/plain", "OK");
  });

   server.begin();
  
  Serial.println("Web server started!");

}

void loop() {

}

String processor(const String& var){
  //Serial.println(var);
  if (var == "SLIDERVALUE"){
    return sliderValue;
  }
  return String();
}
