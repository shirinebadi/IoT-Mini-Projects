void setup() {
  Serial.begin(9600);
  pinMode(D4, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D6, OUTPUT);
 // pinMode(D4, OUTPUT);
}

void loop() {
  int sensorValue = analogRead(A0);
  digitalWrite(D4, HIGH);
  
  float voltage = sensorValue * (100.0 / 1023.0);
  
  Serial.println(voltage);

  if (voltage < 100.0 and voltage > 90.0){
    digitalWrite(D2, HIGH);
    digitalWrite(D4, LOW);
    digitalWrite(D6, HIGH);
  }


  if (voltage < 89.8 and voltage > 65.0){
    digitalWrite(D4, HIGH);
    digitalWrite(D2, LOW);
    digitalWrite(D6, LOW);
  }
  

  delay(3000); 
}
