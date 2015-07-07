const int switchPin = 10;
const int transistorPin = 9;
int switchState = 0;
 
void setup() {
  // put your setup code here, to run once:
  pinMode(switchPin, INPUT);
  pinMode(transistorPin, OUTPUT);
  digitalWrite(transistorPin, LOW);
  Serial.begin(9600);
}

void loop() {
  switchState = digitalRead(switchPin);
  bool active = false;
  
  if(switchState == HIGH) {
    active = true;
    digitalWrite(transistorPin, HIGH);
    delay(2000);
  }
  while(active) {
    switchState = digitalRead(switchPin);
    if(switchState == HIGH) {
      digitalWrite(transistorPin, LOW);
      active = false;  
    }
  }
  // put your main code here, to run repeatedly:
}
