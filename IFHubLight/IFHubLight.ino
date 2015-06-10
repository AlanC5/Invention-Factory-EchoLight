const int ledPin = 4;
const int sensor = A0;
const int switchPin = 3;
const float baseT = 26.0;

int switchstate = 0;
int ledState = 0;
long previouschange = 0;

long interval = 1000;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(switchPin, INPUT);
  Serial.begin(9600);
}

void loop () {
  int sensorValue = analogRead(sensor);
  switchstate = digitalRead(switchPin);
  Serial.print("Sensor Value: ");
  Serial.print(sensorValue);
  float volts = (sensorValue/1024.0) * 5.0;
  Serial.print(" , Volts: ");
  Serial.print(volts);
  float temp = (volts - 0.5) * 100;
  Serial.print(", Degrees C: ");
  Serial.print(temp);
  Serial.println();
  delay(500);
  if(temp > baseT) {
    digitalWrite(ledPin, 1);
  }
  if(switchstate == 0) {
    digitalWrite(ledPin, 0);
  }
  else{
    digitalWrite(ledPin, 1);
  }
}
