#include <Arduino.h>
#include <MFRC522.h>
#include <SPI.h>
#include <LiquidCrystal.h>
#include <Lcduse.h>
#include<SoftwareSerial.h>

//---Software Serial Initialize for communication to ESP8266
SoftwareSerial mySerial(2, 3); // RX, TX

//--LCD initialize
int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = A0, d7 = A1;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
Lcduse ld(lcd);

//Buzzer initialize
int freq = 500;

//RFID initialize
#define RST_PIN         9           // Configurable, see typical pin layout above
#define SS_PIN          10          // Configurable, see typical pin layout above
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

void setup() {

}

void loop() {

}
