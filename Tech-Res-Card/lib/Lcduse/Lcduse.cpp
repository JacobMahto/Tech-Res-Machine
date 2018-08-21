#include "Lcduse.h"
#include "LiquidCrystal.h"



byte customChar20[8] = {
  B10000,
  B10000,
  B00000,
  B00000,
  B00000,
  B10000,
  B10000,
  B00000
};

byte customChar40[8]={
  B11000,
  B10000,
  B01000,
  B00000,
  B01000,
  B10000,
  B11000,
  B00000
  };

byte customChar60[8]{
  B11100,
  B10000,
  B01000,
  B00100,
  B01000,
  B10000,
  B11100,
  B00000
  };

byte customChar80[8]{
  B11110,
  B10000,
  B01000,
  B00100,
  B01000,
  B10000,
  B11110,
  B00000
  };  

 byte customChar100[8] = {
  B11111,
  B10000,
  B01000,
  B00100,
  B01000,
  B10000,
  B11111,
  B00000
}; 


Lcduse::Lcduse(LiquidCrystal& Clcd)
: lcd(Clcd)
{
	
	lcd.begin(16,2);
	lcd.clear();
	lcd.print("Jacob");
	charFromByte();
	lcd.setCursor(0,0);
	lcd.print("vinita");


}

void Lcduse::charFromByte(){
lcd.createChar(0,customChar20);
 lcd.createChar(1,customChar40);
 lcd.createChar(2,customChar60);
 lcd.createChar(3,customChar80);
 lcd.createChar(4,customChar100);
}

void Lcduse::loading(){
	lcd.setCursor(0,1);
   lcd.print("                ");
   
   for(int i=0;i<16;i++){
    for(int j=0;j<5;j++){
      lcd.setCursor(i,1);
      lcd.write((byte)j);
      delay(20);
      }
    }
	
	
	
}


