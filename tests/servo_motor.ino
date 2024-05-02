#include <Servo.h>

#define SERVO 6 // Porta Digital 6 PWM

Servo s; // Variável Servo
int pos; // Posição Servo

void setup ()
{
  s.attach(SERVO);
  Serial.begin(9600);
  s.write(0); // Inicia motor posição zero
}

void loop()
{
  for(pos = 0; pos < 90; pos++)
  {
    s.write(pos);
    delay(15);
  }
delay(1000);
  for(pos = 90; pos >= 0; pos--)
  {
    s.write(pos);
    delay(15);
  }
}