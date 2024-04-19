#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <RFID.h>

#define SDA_DIO 9
#define RESET_DIO 8

const byte ROWS = 4;
const byte COLUMNS = 4;
const byte PINS_ROWS[ROWS] = { 22, 23, 24, 25 };
const byte PINS_COLUMNS[COLUMNS] = { 26, 27, 28, 29 };

const char MATRIX_KEYS[ROWS][COLUMNS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

Keypad this_keypad = Keypad(makeKeymap(MATRIX_KEYS), PINS_ROWS, PINS_COLUMNS, ROWS, COLUMNS);
RFID RC522(SDA_DIO, RESET_DIO);
LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  SPI.begin();
  RC522.init();
}

void loop() {

  char read_keys = this_keypad.getKey();

  if (read_keys) {
    Serial.println(read_keys);
    Serial.println("OIOI");
  }

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Happy ");
  lcd.setCursor(0, 1);
  lcd.print("Happy ");

  lcd.setCursor(0, 2);
  lcd.print("Happy!!!!!!!!!");

  if (RC522.isCard()) {
    RC522.readCardSerial();
    Serial.println("Card detected:");
    for (int i = 0; i < 5; i++) {
      Serial.print(RC522.serNum[i], DEC);
    }
    Serial.println();
    Serial.println();
  }
}
