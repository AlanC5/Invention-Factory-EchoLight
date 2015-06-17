const int RLed = 9;  //Red led 
const int BLed = 10; //Blue led
const int GLed = 11;  //Green led
//const int BLed =
const int Temp_sensor = A1;  //Temperature sensor adjusted
const int Photo_sensor = A0;  //Photoresistor sensor  adjusted
const int Switch_pin = 12;  //Switch
const int Piezo_pin = 8;
 
//Base Values
const float base_Temp = 21.5;
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
  pinMode(BLed, OUTPUT);
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
  //blinks 18 times within 15 seconds --- Mostly used for casual events, emergency event may last indefintely, additional BOOL to confirm if casual or emergency
  if (ledPin == GLed) {
    int blinker = 0;
    while(blinker <= 18) {
      tone(Piezo_pin, 250, 500);//tone(pin, pitch, duration(ms))
      read_Values();
      if (temp > base_Temp) {
        ledPin = RLed;
        blinker = 18;
      }
      digitalWrite(ledPin, HIGH);
      delay(400);
      digitalWrite(ledPin, LOW);
      delay(400);
      switchstate = digitalRead(Switch_pin);
      if (switchstate == 0) {
        blinker = 18;
      }
      blinker++;
    }
  }
  //emergency
  if (ledPin == RLed) {
    bool emergency_On = true;
    while (emergency_On) {
      read_Values();
      tone(Piezo_pin, 500, 500);
      digitalWrite(ledPin, HIGH);
      delay(400);
      digitalWrite(ledPin, LOW);
      delay(400);
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

/*void sound () {
  if(blinker < 18) { 
    tone(Piezo_pin, 250, 1000);//tone(pin, pitch, duration(ms))
    delay(1000);
  }
}*/
