void setup() {
  Serial.begin(9600);

}

void loop() {
  int sensorValue = analogRead(A0);
  
  float voltage = sensorValue * (100.0 / 1023.0);
  
  Serial.println(voltage);  

  delay(3000);  
}
