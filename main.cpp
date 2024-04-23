
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
Stepper myStepper1(stepsPerRevolution, 13, 12, 11, 10);
Stepper myStepper2(stepsPerRevolution, 7, 6, 5, 4);
Servo s;
bool printed;

char products[2] = { 'A', 'B' };
float prices[2] = { 5.0, 4.0 };

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
  printed = false;
}

void option(char key, int index) {
  if (key == '#') {

    float price = prices[index];

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("====================");
    lcd.setCursor(0, 1);
    lcd.print("Aproxime o cartao");
    lcd.setCursor(0, 2);
    lcd.print("Valor: R$ ");
    lcd.print(price);
    Serial.println(price);
    lcd.setCursor(0, 3);
    lcd.print("====================");

    int reading = 0;
    while (!RC522.isCard() && reading < 1000) {
      reading++;
    }

    if (reading >= 1000) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("====================");
      lcd.setCursor(0, 1);
      lcd.print("Pagamento expirado!");
      lcd.setCursor(0, 2);
      lcd.print("Compra cancelada");
      lcd.setCursor(0, 3);
      lcd.print("====================");
      delay(2000);
      printed = false;
      return;
    }

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("====================");
    lcd.setCursor(0, 1);
    lcd.print("Pagamento confirmado!");
    lcd.setCursor(0, 3);
    lcd.print("====================");

    if (index == 0) {
      myStepper1.step(2800);
    } else if (index == 1) {
      myStepper2.step(2800);
    }


    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("====================");
    lcd.setCursor(0, 1);
    lcd.print("Retire seu produto!");
    lcd.setCursor(0, 3);
    lcd.print("====================");

    for (pos = 0; pos < 90; pos++) {
      s.write(pos);
      delay(15);
    }

    delay(7500);

    for (pos = 90; pos >= 0; pos--) {
      s.write(pos);
      delay(15);
    }
    printed = false;
    return;
  }

  else if (key == '*') {
    printed = false;
    return;
  }
}

void loop() {
  char read_keys = this_keypad.getKey();

  if (!printed) {
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("====================");
    lcd.setCursor(0, 1);
    lcd.print("Selecione um produto!");
    lcd.setCursor(0, 3);
    lcd.print("====================");

    printed = true;
  }

  if (read_keys == 'A' || read_keys == 'B') {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("====================");
    lcd.setCursor(0, 1);
    lcd.print("Selecionou produto ");
    lcd.print(read_keys);
    lcd.setCursor(0, 2);
    lcd.print("*-Voltar #-Confirmar");
    lcd.setCursor(0, 3);
    lcd.print("====================");

    int index;
    for (int i = 0; i < sizeof(products); i++) {
      if (products[i] == read_keys) {
        index = i;
        break;
      }
    }

    while (read_keys != '#' && read_keys != '*') {
      read_keys = this_keypad.getKey();
    }

    option(read_keys, index);
  }
}
