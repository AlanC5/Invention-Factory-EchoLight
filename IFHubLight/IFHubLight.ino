const int RLed = 4;//red led
const int GLed = 5;//Green led
const int Tsensor = A1;//temperature sensor
const int Lsensor = A0;//photoresistor sensor
const int switchPin = 3;//switch
const float baseT = 21.0;
const int baseL = 700;

int count = 10;
int switchstate = 0;
int RState = 0;
int GState = 0;
int Lsensorvalue = 0;
int ledPin = 0;
float temp = 0;
float volts = 0;
//long previousMillis = 0;

//long interval = 1000;

void setup() {
  pinMode(RLed, OUTPUT);
  pinMode(GLed, OUTPUT);
  pinMode(switchPin, INPUT);
  Serial.begin(9600);
}

void loop () {
  readValues();
  if (count >= 10) {
    if(Lsensorvalue < baseL) {
      ledPin = GLed;
    }
    else {
      digitalWrite(GLed, 0); 
    }
    if(temp > baseT) {
      ledPin = RLed;
    }
    else {
      digitalWrite(RLed, 0); 
    }
    if (ledPin != 0) {
      int blinker = 0;
      while(blinker <= 30) {
        readValues();
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
      ledPin = 0;
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

void blink(int ledPin) {
  int blinker = 0;
    while(blinker <= 30) {
       digitalWrite(ledPin, HIGH);
       delay(250);
       digitalWrite(ledPin, LOW);
       delay(250);
       switchstate = digitalRead(switchPin);
       if (switchstate == 0) {
         blinker = 30;
         ledPin = 0;
       }
    blinker++;
  }
}

void readValues () {
  //unsigned long currentMillis = millis()
  Lsensorvalue = analogRead(Lsensor);
  int sensorValue = analogRead(Tsensor);
  switchstate = digitalRead(switchPin);
  //Serial.print("Sensor Value: ");
  //Serial.print(sensorValue);
  volts = (sensorValue/1024.0) * 5.0;
  //Serial.print(" , Volts: ");
  //Serial.print(volts);
  temp = (volts - 0.5) * 100;
  Serial.print(", Degrees C: ");
  Serial.print(temp);
  Serial.println();
  Serial.print("Raw Sensor Value \t:");
  Serial.print(Lsensorvalue);
  Serial.println();
}
