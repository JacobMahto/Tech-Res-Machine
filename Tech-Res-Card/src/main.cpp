/*
* Initial Author: ryand1011 (https://github.com/ryand1011)
*
* Reads data written by a program such as "rfid_write_personal_data.ino"
*
* See: https://github.com/miguelbalboa/rfid/tree/master/examples/rfid_write_personal_data
*
* Uses MIFARE RFID card using RFID-RC522 reader
* Uses MFRC522 - Library
* -----------------------------------------------------------------------------------------
*             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
*             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
* Signal      Pin          Pin           Pin       Pin        Pin              Pin
* -----------------------------------------------------------------------------------------
* RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
* SPI SS      SDA(SS)      10            53        D10        10               10
* SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
* SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
* SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
*/
#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>
#include <Lcduse.h>
#include<SoftwareSerial.h>

//---RFID initialize
#define RST_PIN         9           // Configurable, see typical pin layout above
#define SS_PIN          10          // Configurable, see typical pin layout above
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance


//---Software Serial Initialize for communication to ESP8266
SoftwareSerial mySerial(2, 3); // RX, TX

//--LCD initialize
int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = A0, d7 = A1;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
Lcduse ld(lcd);

//Buzzer initialize
int freq = 500;
int beepLed=8;

//mid and name variables
String mid="z",name="z";
int cardReadSuccess=0;
int debug=1;
int cardError=0;
int wifiError=0;

//LED Initialize
int freeLed=A2;

//function prototypes
void readCard();
void beep(int pin, int freq);

//Symbols for LCD
byte customChar[8] = {
  0B00000,
  0B00000,
  0B01010,
  0B10101,
  0B01010,
  0B00000,
  0B00000,
  0B00000
};

//*****************************************************************************************//
void setup() {
  Serial.begin(9600);      // Initialize serial communications with the PC
  mySerial.begin(9600); //Initialize serail communcation with Wi-Fi unit
  SPI.begin();  // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522 card
  if(debug==1){
    Serial.println(F("JRL System is Ready.")); //shows in serial that it is ready to read
  }
  pinMode(freeLed,OUTPUT);//telling that the system is free and has not found any card
  //pinMode(A3,OUTPUT);//telling that the system is busy and is processing the card

  //Starting LCD stuff
  lcd.begin(16, 2);
  lcd.clear();
  //lcd.print("----WELCOME----");
  lcd.print("MK PUBLIC SCHOOL");
  delay(1000);
  lcd.setCursor(0, 2);
  lcd.print(" ENG. BY JACOB");
  //ld.loading();
  pinMode(beepLed, OUTPUT);//initializing LED output

  //Serial.flush();
}

//*****************************************************************************************//
void loop() {
  //Reinitialize global variables
  mid="z";
  name="z";
  cardReadSuccess=0;
  cardError=0;
  wifiError=0;
  digitalWrite(freeLed,HIGH);
  //digitalWrite(A3,LOW);
  // //----------CAUTION :WIFI---------
  // // listen for communication from the ESP8266 and then write it to the serial monitor
  // while (mySerial.available() )   {
  //   Serial.write( mySerial.read() );
  // }
readCard();
  //check and read card
if(cardReadSuccess==1 && mid !=("z") && name !=("z") && cardError==0){

  //print to LCD
    lcd.clear();
    lcd.print("--Welcome--");
    lcd.setCursor(0, 1);
    lcd.print(mid+"--"+name);

//Halting Arduino for a maximum of 3 seconds
//8 seconds is the optimal time to receive response from wifi module
    //---------CAUTION : WIFI
   mySerial.print(mid+"\r\n");//i think the srN should be succedded by \r\n
   unsigned long timeout=millis();
   while(mySerial.available()==0){
     if(millis()-timeout>5000){
       if(debug==1){
         Serial.println(">>>No response from WIFI");
       }
       wifiError=1;
       break;
       //return;
     }
   }
    //to listen from esp8266
    //The below code is working and has been commented just to avoid serial communication , so that time can be saved
String wifiResponse="no";
    while (mySerial.available())   {
      wifiResponse=mySerial.readString();
      wifiResponse.trim();
      if(debug==1){
        Serial.println(wifiResponse);
      }

      }
    //  // readString() is used to read large string at once , reads faster therefore prints all data , whereas due to slowness of read() , it omits many data
//This block is true , when there is timeout on the "myserial"
if(wifiError==1){
  if(debug==1){
    Serial.println("Net Connection Error. Try Again.");
  }
  //lcd info block
  lcd.clear();
  lcd.print("---No Internet---");
  delay(1000);
}
else{
  //This block is true , when the response is received before timeout
  if(wifiResponse=="ok"){
    if(debug==1){
      Serial.print("Success");
    }
    beep(beepLed, freq);
    //lcd info block
    lcd.clear();
    lcd.print("---Success---");
    delay(500);
  }
  else{
    if(debug==1){
      Serial.print("Failed");
    }
    //lcd info block
    lcd.clear();
    lcd.print("---Try Again---");
    delay(500);
  }
}

    lcd.clear();
    lcd.print("MK PUBLIC SCHOOL");
    delay(500);
    lcd.setCursor(0, 2);
    lcd.print(" ENG. BY JACOB");

}
else{
  //lcd.clear();
}

}
//*****************************************************************************************//


//*****************************************FUNCTIONS************************************
//function for reading cards
void readCard(){
  // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  //some variables we need
  byte block;
  byte len;
  MFRC522::StatusCode status;

  //-------------------------------------------

  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
if(debug==1){
  Serial.println(F("**Card Detected:**"));
}


  //-------------------------------------------

  mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid)); //dump some details about the card

  //mfrc522.PICC_DumpToSerial(&(mfrc522.uid));      //uncomment this to see all blocks in hex

  //-------------------------------------------

if(debug==1){
  Serial.print(F("Credentials: "));
}

  byte buffer1[18];

  block = 4;
  len = 18;

  //------------------------------------------- GET FIRST NAME
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 4, &key, &(mfrc522.uid)); //line 834 of MFRC522.cpp file
  if (status != MFRC522::STATUS_OK) {
    if(debug==1){
      Serial.print(F("Authentication failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
    }
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    cardError=1;
    return;
  }

  status = mfrc522.MIFARE_Read(block, buffer1, &len);
  if (status != MFRC522::STATUS_OK) {
    if(debug==1){
      Serial.print(F("Reading failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
    }
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    cardError=1;
    return;
  }

  //PRINT FIRST NAME
  for (uint8_t i = 0; i < 16; i++)
  {
    if (buffer1[i] != 32)
    {
      if(debug==1){
        Serial.write(buffer1[i]);
      }

    }
  }
  if(debug==1){
    Serial.print(" ");
  }


  //---------------------------------------- GET LAST NAME

  byte buffer2[18];
  block = 1;

  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 1, &key, &(mfrc522.uid)); //line 834
  if (status != MFRC522::STATUS_OK) {
    if(debug==1){
      Serial.print(F("Authentication failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
    }
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    cardError=1;
    return;
  }

  status = mfrc522.MIFARE_Read(block, buffer2, &len);
  if (status != MFRC522::STATUS_OK) {
    if(debug==1){
      Serial.print(F("Reading failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
    }
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    cardError=1;
    return;
  }

  //PRINT LAST NAME
  for (uint8_t i = 0; i < 16; i++) {
    if(debug==1){
      Serial.write(buffer2[i] );
    }
  }

  //----------------------------------------
if(debug==1){
  Serial.println(F("\n**End Reading**\n"));
}


//Getting and filtering mid
mid="";
for (char c : buffer1) {
  if(c==' ')break;
  mid += c;
};

//Getting and filtering name
name="";
for (char c : buffer2) {
  if(c==' ')break;
  name += c;
};
// Serial.println(mid);
// Serial.println(name);

cardReadSuccess=1;
  //delay(500); //change value if you want to read cards faster

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();

}

//function for buzzer beep
void beep(int pin, int freq) {
  tone(pin, freq);
  delay(200);
  noTone(pin);
  delay(100);
  tone(pin, freq);
  delay(300);
  noTone(pin);
}
