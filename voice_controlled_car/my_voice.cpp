#include "my_voice.h"

void Voice::setVoiceControl(Car* car){
  this->myCar = car;
}

void Voice::setModeCommand(ModeCommand mode) {
  this->mode = mode;
}

ModeCommand Voice::getModeCommand() {
  return this->mode;
}


void Voice::runFast() {
  if (Serial.available()) {
    uint8_t receiveHexValue = Serial.read();
    if ((receiveHexValue & CONTROL_COMMAND) && getModeCommand() == MODE_1) {
      switch ((ControlCommand)receiveHexValue) {
        case FORWARD:
#ifdef DEBUG
          PRINT_DEBUG(Forward)
#endif
          myCar->setCommand(FORWARD);
          break;
        case BACKWARD:
#ifdef DEBUG
          PRINT_DEBUG(Backward)
#endif
          myCar->setCommand(BACKWARD);
          break;
        case TURN_LEFT:
#ifdef DEBUG
          PRINT_DEBUG(Turn Left)
#endif
          myCar->setCommand(TURN_LEFT);
          break;
        case TURN_RIGHT:
#ifdef DEBUG
          PRINT_DEBUG(Turn Right)
#endif
          myCar->setCommand(TURN_RIGHT);
          break;
        case STOP:
#ifdef DEBUG
          PRINT_DEBUG(Stop)
#endif
          setModeCommand(MODE_DEFAULT);
          myCar->setCommand(STOP);
          break;
      }
      return;
    }
    switch (receiveHexValue) {
      case MODE_1:
#ifdef DEBUG
        PRINT_DEBUG(Mode 1)
#endif
        setModeCommand(MODE_1);
        myCar->setState(VOICE);
        break;
      case MODE_2:
#ifdef DEBUG
        PRINT_DEBUG(Mode 2)
#endif
        setModeCommand(MODE_2);
        myCar->setState(FOLLOW_LINE);
        break;
      case MODE_3:
#ifdef DEBUG
        PRINT_DEBUG(Mode 3)
#endif
        setModeCommand(MODE_3);
        myCar->setState(AVOID_OBSTACLE);
        break;
      case STOP:
#ifdef DEBUG
        PRINT_DEBUG(Stop)
#endif
        setModeCommand(MODE_DEFAULT);
        myCar->setState(DO_NOTHING);
        break;
    }
  }
}

void Voice::setup(int baudRate) {
  Serial.begin(baudRate);
#ifdef DEBUG
  PRINT_DEBUG(voice setup successfully)
#endif
}

void Voice::setBaudRate(int baudRate) {
  Serial.begin(baudRate);
}

void Voice::start() {
  kxnTaskManager.add(this);
  setState(NOTHING);
}

void Voice::stop() {
  kDelay(0);
  setStateIdle();
}