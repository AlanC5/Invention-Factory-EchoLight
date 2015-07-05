#include <SPI.h>

const int BLed = 5;
const int GLed = 6;
const int RLed = 7;

void setup() {
  // put your setup code here, to run once:
  Serial1.begin(9600);
  Serial.begin(9600);
  pinMode(BLed, OUTPUT);
  pinMode(GLed, OUTPUT);
  pinMode(RLed, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial1.available()) {
    char getData = Serial1.read();
    if (getData == 'B') {
      digitalWrite(BLed, HIGH);
      delay(500);
      digitalWrite(BLed, LOW);
      delay(500);
      getData = ' ';
    }
    if (getData == 'G') {
      digitalWrite(GLed, HIGH);
      delay(500);
      digitalWrite(GLed, LOW);
      delay(500);
      getData = ' ';
    }
    if (getData == 'R') {
      digitalWrite(RLed, HIGH);
      delay(500);
      digitalWrite(RLed, LOW);
      delay(500);
      getData = ' ';
    }
    Serial1.print(getData);
    getData = ' ';
  }
}
