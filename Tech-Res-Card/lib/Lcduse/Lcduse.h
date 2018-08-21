#include "Arduino.h"
#include "LiquidCrystal.h"


class Lcduse{
	public:
		Lcduse(LiquidCrystal& Clcd);
		void loading();
		void charFromByte();
		//void init(LiquidCrystal& Clcd);
		LiquidCrystal& lcd;
	private:
		int l;
	
};
