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
int adm[5] = { 228, 227, 210, 207, 26 };
bool flag;

char products[2] = { 'A', 'B' };
int prices[2] = { 0, 0 };

int size = 1;
int **cards;
int *balance;

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

  cards = (int **)malloc(size * sizeof(int *));
  balance = (int *)malloc(size * sizeof(int));

  cards[0] = (int *)malloc(5 * sizeof(int));
  cards[0][0] = 228;
  cards[0][1] = 227;
  cards[0][2] = 210;
  cards[0][3] = 207;
  cards[0][4] = 26;
  balance[0] = 0;
}

bool arrayEquals(int *array1, int *array2) {
  for (int i = 0; i < 5; i++) {
    if (array1[i] != array2[i]) {
      return false;
    }
  }
  return true;
}

int arrayInCards(int *array) {
  for (int i = 0; i < size; i++) {
    if (arrayEquals(cards[i], array)) {
      return i + 1;
    }
  }
  return 0;
}

bool inArray(char input) {

  char array[12] = { '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '*', '#' };
  for (int i = 0; i < 12; i++) {
    if (input == array[i]) {
      return true;
    }
  }
  return false;
}

int toInt(char *array) {
  String str = "";

  for (int i = 0; i < 10; i++) {
    if (array[i] != ' ') {
      str += array[i];
    }
  }

  return str.toFloat();
}

void printLcd(String par1, String par2, String par3, String par4) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("====================");
  lcd.setCursor(0, 1);
  lcd.print(par1);
  lcd.print(par2);
  lcd.setCursor(0, 2);
  lcd.print(par3);
  lcd.print(par4);
  lcd.setCursor(0, 3);
  lcd.print("====================");
}

void addCard() {
  printLcd("Aproxime o cartao!", "", "*-Cancelar", "");

  char read_keys = '0';
  while (read_keys != '*' && !RC522.isCard()) {
    read_keys = this_keypad.getKey();
  }

  if (read_keys == '*') {
    return admAccess();
  } else {

    int input[5];
    RC522.readCardSerial();

    for (int i = 0; i < 5; i++) {
      input[i] = RC522.serNum[i];

      Serial.print(input[i]);
      Serial.print('.');
      Serial.print(RC522.serNum[i]);
      Serial.print('-');
      Serial.println();
    }

    Serial.println(arrayInCards(input));

    if (arrayInCards(input)) {
      printLcd("Cartao ja cadastrado", "", "", "");

      delay(1500);

      return admAccess();
    }

    printLcd("Selecione o Saldo", "", "*-Cancelar", "");

    char input_balance[10] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };

    char read_keys = 'D';
    while (!inArray(read_keys) || read_keys == '#') {
      read_keys = this_keypad.getKey();
    }

    input_balance[0] = read_keys;
    int counter = 1;

    while (read_keys != '*') {
      lcd.clear();
      lcd.print("====================");
      lcd.setCursor(0, 1);
      lcd.print("R$ ");
      for (int i = 0; input_balance[i] != ' '; i++) {
        lcd.print(input_balance[i]);
      }
      lcd.print(".00");
      lcd.setCursor(0, 2);
      lcd.print("#-Confirmar");
      lcd.setCursor(0, 3);
      lcd.print("====================");

      read_keys = 'D';
      while (!inArray(read_keys)) {
        read_keys = this_keypad.getKey();
      }

      if (read_keys == '#') {

        size++;
        cards = (int **)realloc(cards, size * sizeof(int *));
        cards[size - 1] = (int *)malloc(5 * sizeof(int));

        for (int i = 0; i < 5; i++) {
          cards[size - 1][i] = input[i];
        }

        balance = (int *)realloc(balance, size * sizeof(int *));
        balance[size - 1] = toInt(input_balance);

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("====================");
        lcd.setCursor(0, 1);
        lcd.print("Saldo adicionado!");
        lcd.setCursor(0, 2);
        lcd.print("R$ ");
        for (int i = 0; input_balance[i] != ' '; i++) {
          lcd.print(input_balance[i]);
        }
        lcd.print(".00");
        lcd.setCursor(0, 3);
        lcd.print("====================");

        delay(1500);

        return admAccess();
      } else if (read_keys == '*') {
        return admAccess();
      } else {
        input_balance[counter] = read_keys;
        counter++;
      }
    }
    return admAccess();
  }
}

void attBalance() {
  printLcd("Aproxime o cartao!", "", "*-Cancelar", "");

  char read_keys = '0';
  while (read_keys != '*' && !RC522.isCard()) {
    read_keys = this_keypad.getKey();
  }

  if (read_keys == '*') {
    return admAccess();
  } else {

    int input[5];
    RC522.readCardSerial();

    for (int i = 0; i < 5; i++) {
      input[i] = RC522.serNum[i];

      Serial.print(input[i]);
      Serial.print('.');
      Serial.print(RC522.serNum[i]);
      Serial.print('-');
      Serial.println();
    }

    int index = arrayInCards(input) - 1;

    if (!arrayInCards(input)) {
      printLcd("     Cartao nao", "", "     cadastrado", "");

      delay(1500);

      return admAccess();
    } else if (index == 0) {
      printLcd("    Operacao nao", "", "     autorizada", "");
      delay(1500);
      return admAccess();
      
    } else {

      for (int i = 0; i < sizeof(balance); i++) {
        Serial.println(balance[i]);
      }

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("====================");
      lcd.setCursor(0, 1);
      lcd.print("Selecione o saldo");
      lcd.setCursor(0, 2);
      lcd.print("Atual: R$ ");
      lcd.print(balance[index]);
      lcd.print(".00");
      lcd.setCursor(0, 3);
      lcd.print("====================");

      char input_balance[10] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };

      char read_keys = 'D';
      while (!inArray(read_keys) || read_keys == '#') {
        read_keys = this_keypad.getKey();
      }

      input_balance[0] = read_keys;
      int counter = 1;

      while (read_keys != '*') {
        lcd.clear();
        lcd.print("====================");
        lcd.setCursor(0, 1);
        lcd.print("R$ ");
        for (int i = 0; input_balance[i] != ' '; i++) {
          lcd.print(input_balance[i]);
        }
        lcd.print(".00");
        lcd.setCursor(0, 2);
        lcd.print("#-Confirmar");
        lcd.setCursor(0, 3);
        lcd.print("====================");

        read_keys = 'D';
        while (!inArray(read_keys)) {
          read_keys = this_keypad.getKey();
        }

        if (read_keys == '#') {
          balance[index] = toInt(input_balance);

          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("====================");
          lcd.setCursor(0, 1);
          lcd.print("Saldo atualizado!");
          lcd.setCursor(0, 2);
          lcd.print("R$ ");
          for (int i = 0; input_balance[i] != ' '; i++) {
            lcd.print(input_balance[i]);
          }
          lcd.print(".00");
          lcd.setCursor(0, 3);
          lcd.print("====================");

          delay(1500);
          return admAccess();

        } else if (read_keys == '*') {
          return admAccess();

        } else {
          input_balance[counter] = read_keys;
          counter++;
        }
      }
      return admAccess();
    }
  }
}

void admProduct() {
  printLcd("Selecione o produto", "", "A ou B", "");

  char read_keys = '0';
  while (read_keys != '*' && read_keys != 'A' && read_keys != 'B') {
    read_keys = this_keypad.getKey();
  }

  if (read_keys == '*') {
    return admAccess();
  }

  printLcd("Selecionou produto ", String(read_keys), "*-Voltar #-Confirmar", "");

  if (read_keys == '*') {
    return admAccess();
  }

  int index;
  for (int i = 0; i < 2; i++) {
    if (products[i] == read_keys) {
      index = i;
    }
  }

  while (read_keys != '*' && read_keys != '#') {
    read_keys = this_keypad.getKey();
  }

  if (read_keys == '*') {
    return admAccess();
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("====================");
  lcd.setCursor(0, 1);
  lcd.print("Selecione o preco");
  lcd.setCursor(0, 2);
  lcd.print("Atual: R$ ");
  lcd.print(prices[index]);
  lcd.print(".00");
  lcd.setCursor(0, 3);
  lcd.print("====================");

  char input_price[10] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };

  read_keys = 'D';
  while (!inArray(read_keys) || read_keys == '#') {
    read_keys = this_keypad.getKey();
  }


  input_price[0] = read_keys;
  int counter = 1;

  while (read_keys != '*') {
    lcd.clear();
    lcd.print("====================");
    lcd.setCursor(0, 1);
    lcd.print("R$ ");
    for (int i = 0; input_price[i] != ' '; i++) {
      lcd.print(input_price[i]);
    }
    lcd.print(".00");
    lcd.setCursor(0, 2);
    lcd.print("#-Confirmar");
    lcd.setCursor(0, 3);
    lcd.print("====================");

    read_keys = 'D';
    while (!inArray(read_keys)) {
      read_keys = this_keypad.getKey();
    }

    if (read_keys == '#') {
      prices[index] = toInt(input_price);

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("====================");
      lcd.setCursor(0, 1);
      lcd.print("Preco adicionado!");
      lcd.setCursor(0, 2);
      lcd.print("R$ ");
      for (int i = 0; input_price[i] != ' '; i++) {
        lcd.print(input_price[i]);
      }
      lcd.print(".00");
      lcd.setCursor(0, 3);
      lcd.print("====================");

      delay(1500);

      return admAccess();
    } else if (read_keys == '*') {
      return admAccess();
    } else {
      input_price[counter] = read_keys;
      counter++;
    }
  }
}

void admAccess() {

  for (int i = 0; i < size; i++) {
    Serial.print("Cards[");
    Serial.print(i);
    Serial.print("]: ");
    for (int j = 0; j < 5; j++) {
      Serial.print(cards[i][j]);
      Serial.print(" ");
    }
    Serial.println();
  }


  printLcd("1-Cartao 2-Produto", "", "*-Sair", "");

  char read_keys = '0';

  while (read_keys != '1' && read_keys != '*' && read_keys != '2') {
    read_keys = this_keypad.getKey();
  }

  if (read_keys == '*') {
    printed = false;
    return;
  } else if (read_keys == '1') {

    printLcd("1-Alterar saldo", "", "2-Adicionar cartao", "");

    read_keys = '0';
    while (read_keys != '1' && read_keys != '*' && read_keys != '2') {
      read_keys = this_keypad.getKey();
    }

    if (read_keys == '*') {
      return admAccess();
    } else if (read_keys == '1') {
      return attBalance();
    } else if (read_keys == '2') {
      return addCard();
    }
  } else if (read_keys == '2') {
    return admProduct();
  }
}

void option(char key, int index) {
  if (key == '#') {

    int price = prices[index];
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("====================");
    lcd.setCursor(0, 1);
    lcd.print("Aproxime o cartao");
    lcd.setCursor(0, 2);
    lcd.print("Valor: R$ ");
    lcd.print(String(price));
    lcd.print(".00");
    lcd.setCursor(0, 3);
    lcd.print("====================");

    int reading = 0;
    while (!RC522.isCard() && reading < 1000) {
      reading++;
    }

    if (reading >= 1000) {

      printLcd("Pagamento expirado!", "", "Compra cancelada", "");

      delay(1500);
      printed = false;
      return;
    }

    int input[5];
    RC522.readCardSerial();

    for (int i = 0; i < 5; i++) {
      input[i] = RC522.serNum[i];

      Serial.print(input[i]);
      Serial.print('.');
      Serial.print(RC522.serNum[i]);
      Serial.print('-');
      Serial.println();
    }

    if (arrayInCards(input)) {
      int i = arrayInCards(input) - 1;

      if (balance[i] >= price) {
        balance[i] -= price;

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("====================");
        lcd.setCursor(0, 1);
        lcd.print("Pagamento confirmado!");
        lcd.setCursor(0, 2);
        lcd.print("Saldo: R$ ");
        lcd.print(String(balance[i]));
        lcd.print(".00");
        lcd.setCursor(0, 3);
        lcd.print("====================");
      } else {
        printLcd("Saldo insuficiente!", "", "", "");

        delay(1500);

        printed = false;
        return;
      }

      if (index == 0) {
        myStepper1.step(5000);
      } else if (index == 1) {
        myStepper2.step(5000);
      }

      for (pos = 0; pos < 90; pos++) {
        s.write(pos);
        delay(15);
      }

      printLcd("Retire seu produto!", "", "", "");
      delay(7500);

      for (pos = 90; pos >= 0; pos--) {
        s.write(pos);
        delay(15);
      }

      printed = false;
      return;
    } else {
      printLcd("     Cartao nao", "", "     cadastrado", "");

      delay(1500);

      printed = false;
      return;
    }
  }

  else if (key == '*') {
    printed = false;
    return;
  }
}

void loop() {
  char read_keys = this_keypad.getKey();

  if (!printed) {

    printLcd("Selecione um produto!", "", "", "");

    printed = true;
  }

  if (read_keys == 'A' || read_keys == 'B') {

    printLcd("Selecionou produto ", String(read_keys), "*-Voltar #-Confirmar", "");

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

  if (RC522.isCard()) {
    RC522.readCardSerial();

    flag = true;
    for (int i = 0; i < 5; i++) {
      if (RC522.serNum[i] != adm[i]) {
        flag = false;
        break;
      }
    }
    if (flag) {
      admAccess();
    }
  }
}
