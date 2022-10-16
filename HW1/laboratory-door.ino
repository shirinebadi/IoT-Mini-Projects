#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Servo.h>


#define SS_PIN D8
#define RST_PIN D0

const char *ssid     = "";
const char *password = "؛;

WiFiUDP ntpUDP;

Servo servo;

NTPClient timeClient(ntpUDP, "pool.ntp.org");


MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;
// Init array that will store new NUID

byte nuidPICC[4];

void setup() {
 pinMode(D1, OUTPUT); 

 pinMode(D2, OUTPUT);

 digitalWrite(D1, HIGH);

 digitalWrite(D2, LOW);
  
 Serial.begin(115200);
 
 SPI.begin(); // Init SPI bus
 
 rfid.PCD_Init(); // Init MFRC522
 
 Serial.println();
 Serial.print(F("Reader :"));
 rfid.PCD_DumpVersionToSerial();
 
 for (byte i = 0; i < 6; i++) {
   key.keyByte[i] = 0xFF;
 }
 
 Serial.println();
 
 printHex(key.keyByte, MFRC522::MF_KEY_SIZE);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

// Initialize a NTPClient to get time
  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
  timeClient.setTimeOffset(+3);

  servo.attach(2); //D4

  servo.write(0);
  
}

void loop() {
 // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
 if ( ! rfid.PICC_IsNewCardPresent())
   return;
 // Verify if the NUID has been readed
 if ( ! rfid.PICC_ReadCardSerial())
   return;
 Serial.print(F("PICC type: "));
 MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
 Serial.println(rfid.PICC_GetTypeName(piccType));
 // Check is the PICC of Classic MIFARE type
 if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
     piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
     piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
   Serial.println(F("Your tag is not of type MIFARE Classic."));
   return;
 }
 
 if (rfid.uid.uidByte[0] != nuidPICC[0] ||
     rfid.uid.uidByte[1] != nuidPICC[1] ||
     rfid.uid.uidByte[2] != nuidPICC[2] ||
     rfid.uid.uidByte[3] != nuidPICC[3] ) {
   Serial.println(F("A new card has been detected."));
   // Store NUID into nuidPICC array
   for (byte i = 0; i < 4; i++) {
     nuidPICC[i] = rfid.uid.uidByte[i];
   }
   
   Serial.println(F("The NUID tag is:"));
   Serial.print(F("In hex: "));
   printHex(rfid.uid.uidByte, rfid.uid.size);
   Serial.println();
   Serial.print(F("In dec: "));
   printDec(rfid.uid.uidByte, rfid.uid.size);
   Serial.println();

   timeClient.update();

   int currentHour = timeClient.getHours();
   currentHour = currentHour + 3;
   Serial.print("Hour: ");
   Serial.println(currentHour);  

   if(currentHour < 9 and currentHour > 8){
    openDoor();
   }
   else{
    closeDoor();
   }
 }
 
 else Serial.println(F("Card read previously."));
 // Halt PICC
 rfid.PICC_HaltA();
 // Stop encryption on PCD
 rfid.PCD_StopCrypto1();
}


void printHex(byte *buffer, byte bufferSize) {
 for (byte i = 0; i < bufferSize; i++) {
   Serial.print(buffer[i] < 0x10 ? " 0" : " ");
   Serial.print(buffer[i], HEX);
 }
}


void printDec(byte *buffer, byte bufferSize) {
 for (byte i = 0; i < bufferSize; i++) {
   Serial.print(buffer[i] < 0x10 ? " 0" : " ");
   Serial.print(buffer[i], DEC);
 }

 }

void openDoor(){

  delay(1000);

  servo.write(180);
  
  digitalWrite(D1, LOW);

  delay(3000);

  digitalWrite(D1, HIGH);

  servo.write(0);
 
  
 }

void closeDoor(){
   digitalWrite(D2, HIGH);

   delay(3000);

  digitalWrite(D2, LOW);
   
   
}



 
