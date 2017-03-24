#include <LiquidCrystal.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

LiquidCrystal lcd(1, 2, 3, 4, 5, 8);
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key; 

byte nuidPICC[4];
int yellowLed = 7;
int greenLed = 6;

String message = "                ";

void setup() {
  pinMode(yellowLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  
  lcd.begin(16,2);
  delay(100);  
  lcd.setCursor(0,0);
  lcd.println(F("Door lock system"));
  lcd.setCursor(0,1);
  lcd.println(F("by M.L. & A.L.  "));
  delay(2000);
  lcd.clear();  
  
  Serial.begin(115200);
  
  SPI.begin(); // Init SPI bus
  
  rfid.PCD_Init(); // Init MFRC522
    
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
}

void loop() {
  digitalWrite(yellowLed, HIGH);
  lookForCards();
}

void lookForCards() {
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()){
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()){
    return;
  }
  // Print out card
  printDec(mfrc522.uid.uidByte, mfrc522.uid.size);
  // Halt PICC
  mfrc522.PICC_HaltA();
//  lcd.setCursor(0,1);
  printMessage(message);
  delay(50);
  // Stop encryption on PCD
  mfrc522.PCD_StopCrypto1();
} 

void printDec(byte *buffer, byte bufferSize) {
  String rfid = "";
  for (byte i = 0; i < bufferSize; i++) {
    rfid += buffer[i];
  }
  //sends message to serial
  Serial.println("KOODI "+rfid);
  printMessage("Processing......");
  confirmLight();
  delay(1000);
  message = Serial.readStringUntil('\n');
  printMessage(message);
}


void confirmLight(){
  digitalWrite(yellowLed, LOW);
  for(int i = 0; i < 5; i++) {
    digitalWrite(greenLed, HIGH);
    delay(100);
    digitalWrite(greenLed, LOW);
    delay(100);
  }
}


void printMessage(String message) {
  lcd.clear();
//  lcd.setCursor(0,0);
  lcd.println(message);
  delay(2000);
  lcd.clear();
}
