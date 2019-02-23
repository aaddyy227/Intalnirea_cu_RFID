
#include <MFRC522.h> 
#include <SPI.h> 
#include <SD.h> 
#include <RTClib.h>
#define card_sd 4 
#define rfidSCAN 10
#define pinRFID 9
 

File myFile;

MFRC522 rfid(rfidSCAN, pinRFID); 
String uidString;
RTC_DS1307 rtc;
const int ora_checkIN = 9;
const int Minut_checkIN = 5;
int Ora_de_intrare;
int Minutul_de_intrare;
const int rosu = 6;
const int verde = 7;
const int sunet = 5;

void setup() {
  pinMode(rosu, OUTPUT);  
  pinMode(verde, OUTPUT);
  pinMode(sunet, OUTPUT);
  
  
  Serial.begin(9600);
  while(!Serial); 
  SPI.begin(); 
  rfid.PCD_Init(); 
  Serial.print("Initializare card SD");
  if(!SD.begin(card_sd)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("Card initializat !");

  if(!rtc.begin()) {
    Serial.println("Modulul RTC nu e gasit");
    while(1);
  }
  else {
    
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  if(!rtc.isrunning()) {
    Serial.println("Modulul RTC nu functioneaza");
  }
}

void loop() {
  
  if(rfid.PICC_IsNewCardPresent()) {
    readRFID();
    logCard();
    verificare();
  }
  delay(10);
}

void readRFID() {
  rfid.PICC_ReadCardSerial();
  Serial.print("ID: ");
  uidString = String(rfid.uid.uidByte[0]) + " " + String(rfid.uid.uidByte[1]) + " " + 
    String(rfid.uid.uidByte[2]) + " " + String(rfid.uid.uidByte[3]);
  Serial.println(uidString);
 
 
  tone(sunet, 2000); 
  delay(100);        
  noTone(sunet);
  
  delay(100);
}

void logCard() {
  
  digitalWrite(card_sd,LOW);
  
  
  myFile=SD.open("DATA.txt", FILE_WRITE);

 
  if (myFile) {
    Serial.println("Fisier deschis");
    myFile.print(uidString);
    myFile.print(", ");   
    
    DateTime now = rtc.now();
    myFile.print(now.year(), DEC);
    myFile.print('/');
    myFile.print(now.month(), DEC);
    myFile.print('/');
    myFile.print(now.day(), DEC);
    myFile.print(',');
    myFile.print(now.hour(), DEC);
    myFile.print(':');
    myFile.println(now.minute(), DEC);
    
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.println(now.minute(), DEC);
    Serial.println("datele au fost inregistrate");
    myFile.close();

    Ora_de_intrare = now.hour();
    Minutul_de_intrare = now.minute();
  }
  else {
    Serial.println("eroare la deschiderea fisierului data.txt");  
  }
  digitalWrite(card_sd,HIGH);
}

void verificare(){
  if((Ora_de_intrare < ora_checkIN)||((Ora_de_intrare==ora_checkIN) && (Minutul_de_intrare <= Minut_checkIN))){
    digitalWrite(verde, HIGH);
    delay(2000);
    digitalWrite(verde,LOW);
    Serial.println("Bine ai venit !");
  }
  else{
    digitalWrite(rosu, HIGH);
    delay(2000);
    digitalWrite(rosu,LOW);
    Serial.println("Ai intarziat...");
  }
}
