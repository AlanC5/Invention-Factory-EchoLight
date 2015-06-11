const int Photo_sensor = A0;

int sensorValue;
int sensorLow = 1023;
int sensorHigh = 0;

const int ledPin = 13;

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  
  while(millis() < 5000) {
    sensorValue = analogRead(A0);
    if(sensorValue > sensorHigh) {
      sensorHigh = sensorValue;
    }
    if(sensorValue < sensorLow) {
      sensorLow = sensorValue;
    }
  }
  digitalWrite(ledPin, LOW);
}

void loop() {
  sensorValue = analogRead(A0);
  
  int pitch = map(sensorValue, sensorLow, sensorHigh, 50, 4000);
  
  tone(7, 250, 500);
  
  delay(1000);
}

void sound() {
  tone(6, 250, 500);
}
