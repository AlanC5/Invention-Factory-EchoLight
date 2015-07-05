/************************Hardware Related Macros************************************/
#define         MQ_PIN                       (0)     //define which analog input channel you are going to use
#define         RL_VALUE                     (5)     //define the load resistance on the board, in kilo ohms
#define         RO_CLEAN_AIR_FACTOR          (9.83)  //RO_CLEAR_AIR_FACTOR=(Sensor resistance in clean air)/RO,
                                                     //which is derived from the chart in datasheet
 
/***********************Software Related Macros************************************/
#define         CALIBARAION_SAMPLE_TIMES     (50)    //define how many samples you are going to take in the calibration phase
#define         CALIBRATION_SAMPLE_INTERVAL  (500)   //define the time interal(in milisecond) between each samples in the
                                                     //cablibration phase
#define         READ_SAMPLE_INTERVAL         (50)    //define how many samples you are going to take in normal operation
#define         READ_SAMPLE_TIMES            (5)     //define the time interal(in milisecond) between each samples in 
                                                     //normal operation
 
/**********************Application Related Macros**********************************/
#define         GAS_LPG                      (0)
#define         GAS_CO                       (1)
#define         GAS_SMOKE                    (2)
 
const int RLed = 9;  //Red led 
const int BLed = 10; //Blue led
const int GLed = 11;  //Green led
//const int BLed =
const int Temp_sensor = A1;  //Temperature sensor adjusted
const int Photo_sensor = A0;  //Photoresistor sensor  adjusted
const int Gas_sensor = A2;
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
float CO = 0;
float SMOKE = 0;
float LPG = 0;

float           LPGCurve[3]  =  {2.3,0.21,-0.47};   //two points are taken from the curve. 
                                                    //with these two points, a line is formed which is "approximately equivalent"
                                                    //to the original curve. 
                                                    //data format:{ x, y, slope}; point1: (lg200, 0.21), point2: (lg10000, -0.59) 
float           COCurve[3]  =  {2.3,0.72,-0.34};    //two points are taken from the curve. 
                                                    //with these two points, a line is formed which is "approximately equivalent" 
                                                    //to the original curve.
                                                    //data format:{ x, y, slope}; point1: (lg200, 0.72), point2: (lg10000,  0.15) 
float           SmokeCurve[3] ={2.3,0.53,-0.44};    //two points are taken from the curve. 
                                                    //with these two points, a line is formed which is "approximately equivalent" 
                                                    //to the original curve.
                                                    //data format:{ x, y, slope}; point1: (lg200, 0.53), point2: (lg10000,  -0.22)                                                     
float           Ro           =  10;                 //Ro is initialized to 10 kilo ohms 
void setup() {
  pinMode(RLed, OUTPUT);
  pinMode(GLed, OUTPUT);
  pinMode(BLed, OUTPUT);
  pinMode(Switch_pin, INPUT);
  Serial.begin(9600);
    Serial.print("Calibrating...\n");                
  Ro = MQCalibration(MQ_PIN);                       //Calibrating the sensor. Please make sure the sensor is in clean air 
                                                    //when you perform the calibration                    
  Serial.print("Calibration is done...\n"); 
  Serial.print("Ro=");
  Serial.print(Ro);
  Serial.print("kohm");
  Serial.print("\n");
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
  LPG = MQGetGasPercentage(MQRead(MQ_PIN)/Ro,GAS_LPG);
  CO = MQGetGasPercentage(MQRead(MQ_PIN)/Ro,GAS_CO);
  SMOKE = MQGetGasPercentage(MQRead(MQ_PIN)/Ro,GAS_SMOKE);
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
  Serial.print("LPG:"); 
  Serial.print(LPG );
  Serial.print( "ppm" );
  Serial.print("    ");   
  Serial.print("CO:"); 
  Serial.print(CO);
  Serial.print( "ppm" );
  Serial.print("    ");   
  Serial.print("SMOKE:"); 
  Serial.print(SMOKE );
  Serial.print( "ppm" );
  Serial.print("\n");
  Serial.println();
  Serial.println();
}

float MQResistanceCalculation(int raw_adc) {
  return ( ((float)RL_VALUE*(1023-raw_adc)/raw_adc));
}

float MQCalibration(int mq_pin) {
  int i;
  float val=0;
 
  for (i=0;i<CALIBARAION_SAMPLE_TIMES;i++) {            //take multiple samples
    val += MQResistanceCalculation(analogRead(mq_pin));
    delay(CALIBRATION_SAMPLE_INTERVAL);
  }
  val = val/CALIBARAION_SAMPLE_TIMES;                   //calculate the average value
 
  val = val/RO_CLEAN_AIR_FACTOR;                        //divided by RO_CLEAN_AIR_FACTOR yields the Ro 
                                                        //according to the chart in the datasheet 
 
  return val; 
}

float MQRead(int mq_pin) {
  int i;
  float rs=0;
 
  for (i=0;i<READ_SAMPLE_TIMES;i++) {
    rs += MQResistanceCalculation(analogRead(mq_pin));
    delay(READ_SAMPLE_INTERVAL);
  }
 
  rs = rs/READ_SAMPLE_TIMES;
 
  return rs;  
}

int MQGetGasPercentage(float rs_ro_ratio, int gas_id) {
  if ( gas_id == GAS_LPG ) {
     return MQGetPercentage(rs_ro_ratio,LPGCurve);
  } else if ( gas_id == GAS_CO ) {
     return MQGetPercentage(rs_ro_ratio,COCurve);
  } else if ( gas_id == GAS_SMOKE ) {
     return MQGetPercentage(rs_ro_ratio,SmokeCurve);
  }    
 
  return 0;
}

int  MQGetPercentage(float rs_ro_ratio, float *pcurve) {
  return (pow(10,( ((log(rs_ro_ratio)-pcurve[1])/pcurve[2]) + pcurve[0])));
}

/*void sound () {
  if(blinker < 18) { 
    tone(Piezo_pin, 250, 1000);//tone(pin, pitch, duration(ms))
    delay(1000);
  }
}*/
