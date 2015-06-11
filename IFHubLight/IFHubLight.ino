const int RLed = 8;  //Red led
const int GLed = 9;  //Green led
const int Temp_sensor = A1;  //Temperature sensor
const int Photo_sensor = A0;  //Photoresistor sensor
const int Switch_pin = 12;  //Switch

//Base Values
  const float base_Temp = 20.0;
  const int base_Photo = 700;
  
  int count = 10;  //Count for delays
  int switchstate = 0;  //Button state
  int Photo_value = 0;
  int Temp_value = 0;
  int ledPin = 0;  //Contains the active pin
  float volts = 0;  //Sensors read as volts
  float temp = 0;   //Convert volts to temperature

void setup() {
  pinMode(RLed, OUTPUT);
  pinMode(GLed, OUTPUT);
  pinMode(Switch_pin, INPUT);
  Serial.begin(9600);
}

void loop () {
  read_Values();
  if (count >= 10) {
    // Cover photoresistor to turn on Green LED
    if(Photo_value < base_Photo) {
      ledPin = GLed;
    }
    else {
      digitalWrite(GLed, 0); 
    }
    
    // Increase temperature sensor to turn on Red LED
    if(temp > base_Temp) {
      ledPin = RLed;
    }
    else {
      digitalWrite(RLed, 0); 
    }
    
    // Activates blinking lights
    if (ledPin != 0) {
      blink();
    }
    
    //If button is pressed, turn off lights/sound and restart count to allow for delay
    if(switchstate == 0) {
      digitalWrite(ledPin, 0);
      count = 0;
     }
    delay(500);
  }
  
  //This delay allows for 5 second pause in notification system, but it wil continue to read values
  if (count < 10) {
    count++;
    delay(500); 
  }
}

//Configure to override casual with emergencies
void blink() {
  //casual
  //blinks 30 times within 15 seconds --- Mostly used for casual events, emergency event may last indefintely, additional BOOL to confirm if casual or emergency
  if (ledPin == GLed) {
    int blinker = 0;
    while(blinker <= 30) {
      read_Values();
      if (temp > base_Temp) {
        ledPin = RLed;
        blinker = 30;
      }
      digitalWrite(ledPin, HIGH);
      delay(250);
      digitalWrite(ledPin, LOW);
      delay(250);
      switchstate = digitalRead(Switch_pin);
      if (switchstate == 0) {
        blinker = 30;
      }
      blinker++;
    }
  }
  //emergency
  if (ledPin == RLed) {
    bool emergency_On = true;
    while (emergency_On) {
      digitalWrite(ledPin, HIGH);
      delay(250);
      digitalWrite(ledPin, LOW);
      delay(250);
      switchstate = digitalRead(Switch_pin);
      if (switchstate == 0) {
        emergency_On = false;
      }
    }
  }
  ledPin = 0;
}

void read_Values () {
  Photo_value = analogRead(Photo_sensor);
  Temp_value = analogRead(Temp_sensor);
  switchstate = digitalRead(Switch_pin);
  //Serial.print("Sensor Value: ");
  //Serial.print(Temp_value);
  volts = (Temp_value/1024.0) * 5.0;
  //Serial.print(" , Volts: ");
  //Serial.print(volts);
  temp = (volts - 0.5) * 100;
  Serial.print("Degrees C: ");
  Serial.print(temp);
  Serial.println();
  Serial.print("Photoresistor Value: ");
  Serial.print(Photo_value);
  Serial.println();
  Serial.println();
}
