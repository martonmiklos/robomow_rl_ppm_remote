#include <PPMReader.h>

/* DJI Phantom MK1 receiver PPM channel mapping info:
  CH1: Right stick left-right
  CH2: Right stick up-down
  CH3: Left stick up-down
  CH4: Left stick left-right
  CH5: ?
  CH6: Left SW: OFF - 1860 CL- 1520 HL- 1176
  CH7: Right SW: GPS - 1860 ATTI- 1520 ATTI- 1176
  CH8: ?
  CH9: 0
*/
// Initialize a PPMReader on digital pin 3 with 6 expected channels.
byte interruptPin = 3;
byte channelAmount = 9;
PPMReader ppm(interruptPin, channelAmount);

#define FORWARD_THRESHOLD 1600
#define BACKWARD_THRESHOLD 1440

#define USB_DBG

#ifdef USB_DBG
#define DBG(str) Serial.write(str); Serial.println();
#else
#define DBG(str)
#endif

#define MOW_C_PRESS_TIME 10   // 10 * 50 ms
#define MOW_C_RELEASE_TIME 10 // 10 * 50 ms 

enum StickChannels {
  LeftWheelStick = 3,
  RightWheelStick = 2,
  LeftSwitch = 6,
  RightSwitch = 7
};

enum TerminalButton {
  Forward = 0x4000,
  Backward = 0x8000,
  Left = 0x1000,
  Right = 0x2000,
  Rabbit = 0x0001,
  C = 0x0800,
  Go = 0x0400,
  Stop = 0x0002,
  Factory = 0x0004,
  Up = 0x0100,
  Down = 0x200,
};

enum MowActivationState {
  Mow_Idle,
  Mow_C_Pressing,
  Mow_FactoryPressing,
  Mow_C_Released,
};

uint16_t terminalButtonsState = 0;

void calculateButtonsState()
{
  static bool lastMowOn = false;
  static MowActivationState mowState = Mow_Idle;
  static uint8_t mowStateCounter = 0;
  bool mowOn = lastMowOn;
  
  // calculate go
  uint16_t leftStick = ppm.latestValidChannelValue(LeftWheelStick, 0);
  uint16_t rightStick = ppm.latestValidChannelValue(RightWheelStick, 0);

  terminalButtonsState &= ~(Forward | Backward | Left | Right | C | Factory | Rabbit);
  if (rightStick == 0 || leftStick == 0) {
    // invalid readings
    DBG("Invalid values");
    return;
  }

  uint16_t tmp = ppm.latestValidChannelValue(RightSwitch, 0);
  if (tmp != 0) {
    mowOn = tmp < BACKWARD_THRESHOLD;
  }
    
  if (leftStick > FORWARD_THRESHOLD) { // Left forward
    if (rightStick > FORWARD_THRESHOLD) {
      // Both forward -> Forward
      DBG("Forward");
      terminalButtonsState |= Forward;
    } else if (rightStick > BACKWARD_THRESHOLD) {
      // Only left forward -> Right turn
      DBG("Right turn fow");
      terminalButtonsState |= (Right | Forward);
    } else if (rightStick <= BACKWARD_THRESHOLD) {
      // Right forward, left backward -> Right rotate
      DBG("Right rotate");
      terminalButtonsState |= Right;
    }
  } else if (leftStick > BACKWARD_THRESHOLD) { // Left neutral
    if (rightStick > FORWARD_THRESHOLD) {
      // Right forward -> Left turn
      DBG("Left turn fow");
      terminalButtonsState |= (Left | Forward);
    } else if (rightStick <= BACKWARD_THRESHOLD) {
      // Right backward, left idle -> Left turn back
      DBG("Left turn back");
      terminalButtonsState |= (Backward | Left);
    }
  } else if (leftStick <= BACKWARD_THRESHOLD) { // Left backward
    if (rightStick > FORWARD_THRESHOLD) {
      // Both forward -> Left rotate
      DBG("Left rotate");
      terminalButtonsState |= Left;
    } else if (rightStick > BACKWARD_THRESHOLD) {
      // Only left backward -> Right turn
      DBG("Right turn back");
      terminalButtonsState |= (Right | Backward);
    } else if (rightStick <= BACKWARD_THRESHOLD) {
      // Both backward -> Backward
      DBG("Back");
      terminalButtonsState |= Backward;
    }
  }

  tmp = ppm.latestValidChannelValue(LeftSwitch, 0);
  if (tmp != 0 && tmp < BACKWARD_THRESHOLD) {
    DBG("Rabbit");
    terminalButtonsState |= Rabbit;
  }

  if (!mowOn) {
    DBG("Mow off");
    lastMowOn = false;
  } else {
    if (!lastMowOn) {
      // turning mow on
      DBG("Turning mow on press C");
      lastMowOn = true;
      mowState = Mow_C_Pressing;
      terminalButtonsState |= C;
      terminalButtonsState &= Rabbit; // mowing not turned on on City 120 if the Rabbit button is pressed
      mowStateCounter = MOW_C_PRESS_TIME;
    } else {
      switch (mowState) {
      case Mow_C_Pressing:
        terminalButtonsState |= C;
        terminalButtonsState &= Rabbit; // mowing not turned on on City 120 if the Rabbit button is pressed
        mowStateCounter--;
        if (mowStateCounter == 0) {
          DBG("Pressing factory");
          mowStateCounter = MOW_C_RELEASE_TIME;
          mowState = Mow_FactoryPressing;
        }
        break;
      case Mow_FactoryPressing:
        terminalButtonsState |= (C | Factory);
        mowStateCounter--;
        if (mowStateCounter == 0) {
          DBG("Releasing C");
          mowState = Mow_C_Released;
        }
        break;
      case Mow_C_Released:
        terminalButtonsState |= Factory;
        DBG("Mow ON");
        break;
      case Mow_Idle:
        // factory, c flags are cleared upwards already
        DBG("Mow OFF");
        break;
      }
    }
  }
}

void sendRobomowCommand()
{
  static uint8_t dataBuffer[4] = {0x23, 0x00, 0x18, 0xC4};
  dataBuffer[1] = (terminalButtonsState >> 8);
  dataBuffer[2] = ((terminalButtonsState & 0xFF) | 0x18);
  dataBuffer[3] = ~(dataBuffer[0] + dataBuffer[1] + dataBuffer[2]);
  #ifndef USB_DBG
  Serial.write(dataBuffer, 4);
  #endif
  Serial1.write(dataBuffer, 4);
}

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial1.begin(19200);  
}

void loop() 
{
  calculateButtonsState();
  sendRobomowCommand();    
  delay(50);
}
