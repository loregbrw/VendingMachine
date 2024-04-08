
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);


void setup()
{
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  
}

void loop()
{
  
  lcd.clear();
  
  lcd.setCursor(0, 0);
  lcd.print("Happy ");
  lcd.setCursor(0, 1);
  lcd.print("Happy ");

  lcd.setCursor(0, 2);
  lcd.print("Happy!!!!!!!!!");

  delay(5000);
  
}