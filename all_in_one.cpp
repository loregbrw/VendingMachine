#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <SPI.h>
#include <RFID.h>
#include <Stepper.h>

#define SERVO 3
#define RESET_DIO 8
#define SDA_DIO 9

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

int pos;
double stepsPerRevolution = 2048;

Keypad this_keypad = Keypad(makeKeymap(MATRIX_KEYS), PINS_ROWS, PINS_COLUMNS, ROWS, COLUMNS);
RFID RC522(SDA_DIO, RESET_DIO);
LiquidCrystal_I2C lcd(0x27, 20, 4);
Stepper myStepper1(stepsPerRevolution, 10, 11, 12, 13);
Stepper myStepper2(stepsPerRevolution, 4, 5, 6, 7);
Servo s;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  SPI.begin();
  RC522.init();
  s.attach(SERVO);
  s.write(0);
  myStepper1.setSpeed(10);
  myStepper2.setSpeed(10);
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

  // for (pos = 0; pos < 90; pos++) {
  //   s.write(pos);
  //   delay(15);
  // }
  // delay(1000);
  // for (pos = 90; pos >= 0; pos--) {
  //   s.write(pos);
  //   delay(15);
  // }

  // myStepper1.step(stepsPerRevolution);
  // myStepper2.step(stepsPerRevolution);

}
