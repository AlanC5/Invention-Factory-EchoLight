const int ledPin = 4;
const int sensor = A0;
const int switchPin = 3;
const float baseT = 21.0;

int count = 10;
int switchstate = 0;
int ledState = 0;
//long previousMillis = 0;

//long interval = 1000;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(switchPin, INPUT);
  Serial.begin(9600);
}

void loop () {
  //unsigned long currentMillis = millis()
  int sensorValue = analogRead(sensor);
  switchstate = digitalRead(switchPin);
  //Serial.print("Sensor Value: ");
  //Serial.print(sensorValue);
  float volts = (sensorValue/1024.0) * 5.0;
  //Serial.print(" , Volts: ");
  //Serial.print(volts);
  float temp = (volts - 0.5) * 100;
  Serial.print(", Degrees C: ");
  Serial.print(temp);
  Serial.println();
  if (count >= 10) {
    if(temp > baseT) {
      int blinker = 0;
      while(blinker <= 30) {
        digitalWrite(ledPin, HIGH);
        delay(250);
        digitalWrite(ledPin, LOW);
        delay(250);
        switchstate = digitalRead(switchPin);
        if (switchstate == 0) {
          blinker = 30;
        }
      blinker++;
      }
      //digitalWrite(ledPin, 1);
    }
    else {
      digitalWrite(ledPin, 0); 
    }
    if(switchstate == 0) {
      digitalWrite(ledPin, 0);
      count = 0;
     }
    delay(500);
  }
  if (count < 10) {
    count++;
    delay(500); 
  }
}

void blink() {
  int blinker = 0;
  while(blinker <= 30) {
    digitalWrite(ledPin, HIGH);
    delay(250);
    digitalWrite(ledPin, LOW);
    delay(250);
    switchstate = digitalRead(switchPin);
    if (switchstate = 1) {
      blinker = 30;
    }
    blinker++;
  }
}
