/************************************************************************
*
* Test of the Pmod module
*
*************************************************************************
* Description: Pmod_CLP
* Message "Test module Pmod Digilent partner of Lextronic" will be show on LCD.
*
* Material
* 1. Arduino Uno
* 2. Pmod CLP
*
**************************************************/
// Affectation of pins
#define en 7
#define rs 6
#define d7 5
#define d6 4
#define d5 3
#define d4 2

#include<LiquidCrystal.h> // Add LiquidCrystal library
LiquidCrystallcd(rs, en, d4, d5, d6, d7); // creation of object LCD

voidsetup()
{
lcd.begin(16,2); // initialization of LCD object
}

voidloop()
{
lcd.clear(); // Erase screen
lcd.print("Test module Pmod");
lcd.setCursor(4,1); // cursor is on line 2 and columm 5
lcd.print("Digilent");
delay(3000);
lcd.clear();
lcd.setCursor(1,0); // cursor is on line 1 and columm 2
lcd.print("partner of");
lcd.setCursor(3,1); // cursor is on line 2 and columm 4
lcd.print("Lextronic");
delay(3000);
}