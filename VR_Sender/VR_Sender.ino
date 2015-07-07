#include "Arduino.h"
#if !defined(SERIAL_PORT_MONITOR)
  #error "Arduino version not supported. Please update your IDE to the latest version."
#endif

#if defined(SERIAL_PORT_USBVIRTUAL)
  // Shield Jumper on HW (for Leonardo and Due)
  #define port SERIAL_PORT_HARDWARE
  #define pcSerial SERIAL_PORT_USBVIRTUAL
#else
  // Shield Jumper on SW (using pins 12/13 or 8/9 as RX/TX)
  #include "SoftwareSerial.h"
  SoftwareSerial port(12, 13);
  #define pcSerial SERIAL_PORT_MONITOR
#endif

#include "EasyVR.h"

#include <SPI.h>
EasyVR easyvr(port);

//Groups and Commands
enum Groups
{
  GROUP_1  = 1,
  GROUP_2  = 2,
};

enum Group1 
{
  G1_ECHO_TALK = 0,
  G1_ECHO_1 = 1,
  G1_ECHO_2 = 2,
  G1_ECHO_3 = 3,
  G1_ECHO_LIGHTS_OUT = 4,
  G1_SOUND_1 = 5,
  G1_SOUND_2 = 6,
  G1_SOUND_3 = 7,
};

enum Group2
{
  G2_ECHO_OVER = 0,
};

int8_t group, idx;

//LED require multi-colored
const int GreenLED = 7;
const int RedLED =  6;
const int BlueLED = 5;

void setup()
{
  pinMode(GreenLED, OUTPUT);
  pinMode(RedLED, OUTPUT);
  pinMode(BlueLED, OUTPUT);
  // setup PC serial port
  pcSerial.begin(9600);

  // setup serial port for xbee
  Serial1.begin(9600);
  // bridge mode?
  int mode = easyvr.bridgeRequested(pcSerial);
  switch (mode)
  {
  case EasyVR::BRIDGE_NONE:
    // setup EasyVR serial port
    port.begin(9600);
    // run normally
    pcSerial.println(F("---"));
    pcSerial.println(F("Bridge not started!"));
    break;
    
  case EasyVR::BRIDGE_NORMAL:
    // setup EasyVR serial port (low speed)
    port.begin(9600);
    // soft-connect the two serial ports (PC and EasyVR)
    easyvr.bridgeLoop(pcSerial);
    // resume normally if aborted
    pcSerial.println(F("---"));
    pcSerial.println(F("Bridge connection aborted!"));
    break;
    
  case EasyVR::BRIDGE_BOOT:
    // setup EasyVR serial port (high speed)
    port.begin(115200);
    // soft-connect the two serial ports (PC and EasyVR)
    easyvr.bridgeLoop(pcSerial);
    // resume normally if aborted
    pcSerial.println(F("---"));
    pcSerial.println(F("Bridge connection aborted!"));
    break;
  }

  while (!easyvr.detect())
  {
    Serial.println("EasyVR not detected!");
    delay(1000);
  }

  easyvr.setPinOutput(EasyVR::IO1, LOW);
  Serial.println("EasyVR detected!");
  easyvr.setTimeout(0);
  easyvr.setLanguage(0);

  group = 1; //<-- start group (customize)
}

void action();

void loop()
{
  easyvr.setPinOutput(EasyVR::IO1, HIGH); // LED on (listening)

  Serial.print("Say a command in Group ");
  Serial.println(group);
  easyvr.recognizeCommand(group);

  do
  {
    // can do some processing while waiting for a spoken command

  }
  while (!easyvr.hasFinished());
  
  easyvr.setPinOutput(EasyVR::IO1, LOW); // LED off

  idx = easyvr.getWord();
  if (idx >= 0)
  {
    // built-in trigger (ROBOT)
    // group = GROUP_X; <-- jump to another group X
    return;
  }
  idx = easyvr.getCommand();
  if (idx >= 0)
  {
    // print debug message
    uint8_t train = 0;
    char name[32];
    Serial.print("Command: ");
    Serial.print(idx);
    if (easyvr.dumpCommand(group, idx, name, train))
    {
      Serial.print(" = ");
      Serial.println(name);
    }
    else
      Serial.println();
    easyvr.playSound(0, EasyVR::VOL_FULL);
    // perform some action
    action();
  }
  else // errors or timeout
  {
    if (easyvr.isTimeout())
      Serial.println("Timed out, try again...");
    int16_t err = easyvr.getError();
    if (err >= 0)
    {
      Serial.print("Error ");
      Serial.println(err, HEX);
    }
  }
}

void action()
{
    switch (group)
    {
    case GROUP_1:
      switch (idx)
      {
      case G1_ECHO_TALK:
      //Open up intercom mode
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        Reset();
        digitalWrite(RedLED, HIGH);
        digitalWrite(GreenLED, HIGH);
        digitalWrite(BlueLED, HIGH);
        // Send 'A' character and turn on walkie talkie
        // Other Arduino well doesn't need to do anything
         Serial1.print('A');
        group = 2;
        break;
      case G1_ECHO_1:
        //Tea Kettle Whistle
        Reset();
        digitalWrite(GreenLED, HIGH);
        Training (1, 5);
        break;
      case G1_ECHO_2:
        //Door Bell Ring
        Reset();
        digitalWrite(BlueLED, HIGH);
        Training (1, 6);
        break;
      case G1_ECHO_3:
        //Phone Ring Tone
        Reset();     
        analogWrite(RedLED, 255);
        digitalWrite(BlueLED, HIGH);
        Training (1, 7);
        break;
      case G1_ECHO_LIGHTS_OUT:
        Reset();
        // "C" to close off walkie and turn off lights
        Serial1.print('C');
        break;
      case G1_SOUND_1:
        //Tea Kettle Whistle
        Reset();
        Serial1.print('X');
        digitalWrite(GreenLED, HIGH);
        easyvr.playSound(3, EasyVR::VOL_FULL);
        break;
      case G1_SOUND_2:
        //Door Bell Ring
        Reset();
        Serial1.print('Y');
        digitalWrite(BlueLED, HIGH);
        easyvr.playSound(1, EasyVR::VOL_FULL);
        break;
      case G1_SOUND_3:
        //Phone Ring Tone
        Reset();
        Serial1.print('Z');
        analogWrite(RedLED, 255);
        digitalWrite(BlueLED, HIGH);
        easyvr.playSound(2, EasyVR::VOL_FULL);
        break;
      }
      break;
    case GROUP_2:
      switch (idx)
      {
      case G2_ECHO_OVER:
        //Close off intercom mode
        Reset();
        group = 1;
        // "C" to close off walkie and turn off lights
        Serial1.print('C');
        break;
      }
      break;
    }
}

//Erase and Start Training
void Training (int8_t C_Group, int8_t C_Index) {
  if(easyvr.eraseCommand(C_Group, C_Index)) {
    Serial.println("Erase existing training");
  }
  Serial.println("Start Training");
  easyvr.trainCommand(C_Group, C_Index);
  while (!easyvr.hasFinished()); // waits for command to be trained
    int16_t err = easyvr.getError();
    if (err >= 0)
    {
      Serial.print("Error ");
      Serial.println(err, HEX);         
    }
    else 
    {
      Serial.println("Successful Training");
    }
}

//Disables any lights and sounds
void Reset () {
  digitalWrite(BlueLED, LOW);
  digitalWrite(RedLED, LOW);
  digitalWrite(GreenLED, LOW);
}


