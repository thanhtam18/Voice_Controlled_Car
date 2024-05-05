#include "my_car.h"

void Car::loop() {
  if (getState() != getPreviousState())
    carStop();
  switch (getState()) {
    case VOICE:
      voiceMode();
      setPreviousState(VOICE);
      break;
    case FOLLOW_LINE:
#ifdef DEBUG
      PRINT_DEBUG(Follow line)
#endif
      setCommand(STOP);
      followLineMode();
      setPreviousState(FOLLOW_LINE);
      break;
    case AVOID_OBSTACLE:
#ifdef DEBUG
      PRINT_DEBUG(Avoid Obstacle)
#endif
      setCommand(STOP);
      avoidObstacleMode();
      setPreviousState(AVOID_OBSTACLE);
      break;
    case DO_NOTHING:
#ifdef DEBUG
      PRINT_DEBUG(Stop)
#endif
      carStop();
  }
  kDelay(1000);
}

void Car::voiceMode() {
  switch (getCommand()) {
    case FORWARD:
#ifdef DEBUG
      PRINT_DEBUG(Forward)
#endif
      goForward();
      break;
    case BACKWARD:
#ifdef DEBUG
      PRINT_DEBUG(Backward)
#endif
      goBackward();
      break;
    case TURN_LEFT:
#ifdef DEBUG
      PRINT_DEBUG(Turn left)
#endif
      turnLeft();
      break;
    case TURN_RIGHT:
#ifdef DEBUG
      PRINT_DEBUG(Tunr right)
#endif
      turnRight();
      break;
    case STOP:
#ifdef DEBUG
      PRINT_DEBUG(Stop)
#endif
      carStop();
      break;
  }
}

void Car::followLineMode() {
  int leftSensorAnalog = analogRead(leftSensorPin);
  int midSensorAnalog = analogRead(midSensorPin);
  int rightSensorAnalog = analogRead(rightSensorPin);
}

void Car::avoidObstacleMode() {
  kDelay(60);
  static bool isAnotherWay = true;
  float distance = this->ultrasonic->dist();
  if (distance > 20.0 && isAnotherWay) {
    goForward();
  } else {
    isAnotherWay = false;
    carStop();
    static uint8_t index = 1;
    switch (index) {
      case 1:
        this->servo->write(180);
        kDelay(500);
        distance = this->ultrasonic->dist();
        this->servo->write(90);
        kDelay(500);
        if (distance > 20.0) {
          turnLeft();
          isAnotherWay = true;
          index = 1;
        } else
          index++;
        break;
      case 2:
        this->servo->write(0);
        kDelay(500);
        distance = this->ultrasonic->dist();
        this->servo->write(90);
        kDelay(500);
        if (distance > 20.0) {
          turnRight();
          isAnotherWay = true;
          index = 1;
        } else
          index++;
        break;
      case 3:
        goBackward();
        kDelay(500);
        carStop();
        index = 1;
        break;
    }
  }
}



void Car::start() {
  kxnTaskManager.add(this);
  setState(VOICE);
}
void Car::stop() {
  kDelay(0);
  setStateIdle();
}

void Car::setCommand(ControlCommand cmd) {
  this->cmd = cmd;
}

ControlCommand Car::getCommand() {
  return this->cmd;
}

void Car::setPreviousState(enumStateCar state) {
  this->previousSate = state;
}

enumStateCar Car::getPreviousState() {
  return this->previousSate;
}

void Car::setupMotor() {
  this->initSpeedMotorLeft = 140;
  this->initSpeedMotorRight = 140;
  this->enable1LeftPin = 8;
  this->enable1RightPin = 4;
  this->enable2LeftPin = 9;
  this->enable2RightPin = 7;
  this->controlSpeedLeft = 6;
  this->controlSpeedRight = 5;

  pinMode(this->enable1LeftPin, OUTPUT);
  pinMode(this->enable2LeftPin, OUTPUT);
  pinMode(this->controlSpeedLeft, OUTPUT);

  pinMode(this->enable1RightPin, OUTPUT);
  pinMode(this->enable2RightPin, OUTPUT);
  pinMode(this->controlSpeedRight, OUTPUT);
}
void Car::setupMotorLeft(uint8_t enable1LeftPin, uint8_t enable2LeftPin, uint8_t controlSpeedLeft, uint8_t initSpeedMotorLeft) {
  this->enable1LeftPin = enable1LeftPin;
  this->enable2LeftPin = enable2LeftPin;
  this->controlSpeedLeft = controlSpeedLeft;
  this->initSpeedMotorLeft = initSpeedMotorLeft;
  pinMode(this->enable1LeftPin, OUTPUT);
  pinMode(this->enable2LeftPin, OUTPUT);
  pinMode(this->controlSpeedLeft, OUTPUT);
}
void Car::setupMotorRight(uint8_t enable1RightPin, uint8_t enable2RightPin, uint8_t controlSpeedRight, uint8_t initSpeedMotorRight) {
  this->enable1RightPin = enable1RightPin;
  this->enable2RightPin = enable2RightPin;
  this->controlSpeedRight = controlSpeedRight;
  this->initSpeedMotorRight = initSpeedMotorRight;
  pinMode(this->enable1RightPin, OUTPUT);
  pinMode(this->enable2RightPin, OUTPUT);
  pinMode(this->controlSpeedRight, OUTPUT);
}
void Car::setupLineSensor() {
  this->leftSensorPin = A1;
  this->midSensorPin = A2;
  this->rightSensorPin = A3;

  pinMode(this->leftSensorPin, INPUT);
  pinMode(this->midSensorPin, INPUT);
  pinMode(this->rightSensorPin, INPUT);
}
void Car::setupLineSensor(uint8_t leftSensorPin, uint8_t rightSensorPin) {
  this->leftSensorPin = leftSensorPin;
  this->rightSensorPin = rightSensorPin;
  pinMode(this->leftSensorPin, INPUT);
  pinMode(this->rightSensorPin, INPUT);
}
void Car::setupLineSensor(uint8_t leftSensorPin, uint8_t midSensorPin, uint8_t rightSensorPin) {
  this->leftSensorPin = leftSensorPin;
  this->midSensorPin = midSensorPin;
  this->rightSensorPin = rightSensorPin;
  pinMode(this->leftSensorPin, INPUT);
  pinMode(this->midSensorPin, INPUT);
  pinMode(this->rightSensorPin, INPUT);
}

void Car::setupUltrasonic(uint8_t ultrasonicTricPin, uint8_t ultrasonicEchoPin, HCSR04 *ultrasonic) {
  this->ultrasonic = ultrasonic;
  this->ultrasonic->setupPin(ultrasonicEchoPin, ultrasonicTricPin);
}

void Car::setupServo(uint8_t servoPin, Servo *servo) {
  this->servo = servo;
  this->servo->attach(servoPin);
  this->servo->write(90);
}

void Car::goForward() {
  analogWrite(controlSpeedLeft, initSpeedMotorLeft);
  analogWrite(controlSpeedRight, initSpeedMotorRight);
  digitalWrite(enable1LeftPin, HIGH);
  digitalWrite(enable2LeftPin, LOW);
  digitalWrite(enable1RightPin, LOW);
  digitalWrite(enable2RightPin, HIGH);
}

void Car::goBackward() {
  analogWrite(controlSpeedLeft, initSpeedMotorLeft);
  analogWrite(controlSpeedRight, initSpeedMotorRight);
  digitalWrite(enable1LeftPin, LOW);
  digitalWrite(enable2LeftPin, HIGH);
  digitalWrite(enable1RightPin, HIGH);
  digitalWrite(enable2RightPin, LOW);
}

void Car::turnLeft() {
  analogWrite(controlSpeedLeft, 250);
  analogWrite(controlSpeedRight, 250);
  digitalWrite(enable1LeftPin, LOW);
  digitalWrite(enable2LeftPin, HIGH);
  digitalWrite(enable1RightPin, LOW);
  digitalWrite(enable2RightPin, HIGH);
  kDelay(600);
  digitalWrite(enable2LeftPin, LOW);
  digitalWrite(enable2RightPin, LOW);
}

void Car::turnRight() {
  analogWrite(controlSpeedLeft, 250);
  analogWrite(controlSpeedRight, 250);
  digitalWrite(enable1LeftPin, HIGH);
  digitalWrite(enable2LeftPin, LOW);
  digitalWrite(enable1RightPin, HIGH);
  digitalWrite(enable2RightPin, LOW);
  kDelay(600);
  digitalWrite(enable2LeftPin, LOW);
  digitalWrite(enable2RightPin, LOW);
}

void Car::carStop() {
  analogWrite(controlSpeedLeft, 255);
  analogWrite(controlSpeedRight, 255);
  digitalWrite(enable1LeftPin, HIGH);
  digitalWrite(enable2LeftPin, HIGH);
  digitalWrite(enable1RightPin, HIGH);
  digitalWrite(enable2RightPin, HIGH);
  analogWrite(controlSpeedLeft, 0);
  analogWrite(controlSpeedRight, 0);
}
