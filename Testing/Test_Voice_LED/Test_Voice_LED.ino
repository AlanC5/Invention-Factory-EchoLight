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
EasyVR easyvr(port);
//Groups and Commands
enum Groups
{
  GROUP_0  = 0,
  GROUP_1  = 1,
  GROUP_2  = 2,
};
enum Group0
{
  G0_HUBLIGHT = 0,
};
enum Group1
{
  G1_ON = 0,
  G1_OFF = 1,
};
enum Group2
{
  G2_DOORBELL = 0,
  G2_KETTLE = 1,
};

int8_t group, idx;
//LED
const int GreenLED = 7;
const int RedLED =  6;
void setup()
{
  pinMode(GreenLED, OUTPUT);
  pinMode(RedLED, OUTPUT);
  // setup PC serial port
  pcSerial.begin(9600);
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
  easyvr.setTimeout(5);
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
  case GROUP_0:
    switch (idx)
    {
    case G0_HUBLIGHT:
      // write your action code here
      // group = GROUP_X; <-- or jump to another group X for composite commands
      group = GROUP_1;
      break;
    }
    break;
  case GROUP_1:
    Serial.println("Say ON or OFF");
    switch (idx)
    {
    case G1_ON:
      digitalWrite(GreenLED, HIGH);
      // write your action code here
      // group = GROUP_X; <-- or jump to another group X for composite commands
      break;
    case G1_OFF:
      digitalWrite(GreenLED, LOW);
      // write your action code here
      // group = GROUP_X; <-- or jump to another group X for composite commands
      break;
    }
    break;
  case GROUP_2:
    switch (idx)
    {
    case G2_DOORBELL:
      // write your action code here
      // group = GROUP_X; <-- or jump to another group X for composite commands
      break;
    case G2_KETTLE:
      // write your action code here
      // group = GROUP_X; <-- or jump to another group X for composite commands
      break;
    }
    break;
  }
}


