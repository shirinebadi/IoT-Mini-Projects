const int ledPin = 2; 

void setup() {

  Serial.begin(9600);
  
}

void loop() {

  int sensorValue = analogRead(A0);
  
  float voltage = sensorValue * (255.0 / 1023.0);
  
  Serial.println(255 - voltage);  
  
  analogWrite(ledPin, voltage);

  delay(4000);
}
