long duration;
int distance;

void setup() {
pinMode(D1, OUTPUT); 
pinMode(D2, OUTPUT);
pinMode(D5, OUTPUT);
pinMode(D6, OUTPUT);
pinMode(D7, OUTPUT);
pinMode(D8, OUTPUT);
pinMode(D0, INPUT); 

Serial.begin(9600); 

digitalWrite(D2, HIGH);
digitalWrite(D8, HIGH);
digitalWrite(D5, HIGH);
digitalWrite(D7, HIGH);
digitalWrite(D6, HIGH);
}

void loop() {

digitalWrite(D1, LOW);
delayMicroseconds(2);
digitalWrite(D1, HIGH);
delayMicroseconds(10);
digitalWrite(D1, LOW);

duration = pulseIn(D0, HIGH);

distance= duration*0.034/2;

Serial.println(distance);

delay(2000);

if(distance<5 and distance>1){
    digitalWrite(D2, LOW);
    digitalWrite(D8, HIGH);
    digitalWrite(D5, HIGH);
    digitalWrite(D7, HIGH);
    digitalWrite(D6, HIGH);
}


if(distance<8 and distance>4){
    digitalWrite(D5, LOW);
    digitalWrite(D2, HIGH);
    digitalWrite(D8, HIGH);
    digitalWrite(D7, HIGH);
    digitalWrite(D6, HIGH);
}


if(distance<10 and distance>7){
  
  digitalWrite(D6, LOW);
  digitalWrite(D2, HIGH);
  digitalWrite(D8, HIGH);
  digitalWrite(D5, HIGH);
  digitalWrite(D7, HIGH);
}



if(distance<12 and distance>9){

   digitalWrite(D7, LOW);
   digitalWrite(D2, HIGH);
   digitalWrite(D8, HIGH);
   digitalWrite(D5, HIGH);
   digitalWrite(D6, HIGH);
}


if(distance<14 and distance>11){

  digitalWrite(D8, LOW);
  digitalWrite(D2, HIGH);
  digitalWrite(D5, HIGH);
  digitalWrite(D6, HIGH);
  digitalWrite(D7, HIGH);
  
}



}
