#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <SPI.h>
#include <MFRC522.h>

constexpr uint8_t RST_PIN = 5; 
constexpr uint8_t SS_PIN = 4; 

MFRC522 rfid(SS_PIN, RST_PIN); 
MFRC522::MIFARE_Key key;

byte nuidPICC[4];

const char* ssid     = "NodeMCU";     
const char* password = "12345678"; 

ESP8266WebServer server(80);   //instantiate server at port 80 (http port)

String page = "";
String namepage = "";

void setup(void){
   
  delay(5000);

  IPAddress local_ip(192,168,1,5);
  IPAddress gateway(192,168,1,1);
  IPAddress subnet(255,255,255,0);
  WiFi.softAPConfig(local_ip, gateway, subnet);
 
  Serial.print("Server IP address: ");
  Serial.println(WiFi.softAPIP());
  
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
 Serial.println(F("This code scan the MIFARE Classic NUID."));
 
 Serial.print(F("Using the following key:"));
 printHex(key.keyByte, MFRC522::MF_KEY_SIZE);

  WiFi.softAP(ssid,password);

  delay(5000);

  Serial.print("Connected to ");
  Serial.println(ssid);
  
   
  server.on("/hi", [](){
    server.send(200, "text/html", page);
  });
  
  server.on("/play", [](){
  Serial.println("PLAYING");
  server.send(200, "text/html", page);
  });

  server.on("/pause", [](){
  Serial.println("PAUSE");
  server.send(200, "text/html", page);
  });

  server.on("/down", [](){
  Serial.println("DOWN");
  server.send(200, "text/html", page);
  });

  server.on("/up", [](){
  Serial.println("UP");
  server.send(200, "text/html", page);
  });

  server.on("/name", [](){
   delay(2000);

   boolean tread = false;

  while(tread != true){
    Serial.print("try again");
    
   if ( ! rfid.PICC_IsNewCardPresent())
     return;
  
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

     String content = "";
      
     tread = true;

     for (byte i = 0; i < rfid.uid.size; i++) {

     content.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(rfid.uid.uidByte[i], HEX));

     }

     content.toUpperCase();
     if (content.substring(1) == "33 6F F3 1A") {

      server.send(200, "text/html", namepage);

     }

     else{
      server.send(404);
     }
     
   }
   
   else Serial.println(F("Card read previously."));
   // Halt PICC
   rfid.PICC_HaltA();
   // Stop encryption on PCD
   rfid.PCD_StopCrypto1();
   
  }
   server.send(200, "text/html", namepage);
  });

  
  server.begin();
  
  Serial.println("Web server started!");

   page = "<p><a href=\"play\"><button>play</button></a>&nbsp;<a href=\"pause\"><button>pause</button></a></p>&nbsp;<p><a href=\"up\"><button>+</button></a>&nbsp;<a href=\"down\"><button>-</button></a></p>&nbsp;<p><a href=\"name\"><button>Name</button></a></p>";

   namepage = "<h> You just watched: Vagabond 1985</h>";
}
 
void loop(void){
  server.handleClient();
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
