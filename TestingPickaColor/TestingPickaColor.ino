#include <SPI.h>

const int switchB = 8;
const int switchG = 9;
const int switchR = 10;
const int BLed = 11;
const int GLed = 12;
const int RLed = 13;

int switchStateB = 0;
int switchStateG = 0;
int switchStateR = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(switchB, INPUT);
  pinMode(switchG, INPUT);
  pinMode(switchR, INPUT);
  pinMode(BLed, OUTPUT);
  pinMode(GLed, OUTPUT);
  pinMode(RLed, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  switchStateB = digitalRead(switchB);
  if (switchStateB == HIGH) {
    digitalWrite(BLed, HIGH);
    delay(500);
    digitalWrite(BLed, LOW);
    Serial.print('B');
  }
  switchStateG = digitalRead(switchG);
  if (switchStateG == HIGH) {
    digitalWrite(GLed, HIGH);
    delay(500);
    digitalWrite(GLed, LOW);
    Serial.print('G');
  }
    switchStateR = digitalRead(switchR);
  if (switchStateR == HIGH) {
    digitalWrite(RLed, HIGH);
    delay(500);
    digitalWrite(RLed, LOW);
    Serial.print('R');
  }
}
