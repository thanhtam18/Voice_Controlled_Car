#ifndef __CAR_H
#define __CAR_H

#include "kxnTask.h"
#include "debug.h"
#include "my_HCSR04.h"
#include "Servo.h"

#define DEBUG

DEFINE_TASK_STATE(Car){
  DO_NOTHING,
  VOICE,
  FOLLOW_LINE,
  AVOID_OBSTACLE
};

typedef enum{
  FORWARD = 0x0A,
  BACKWARD,
  TURN_LEFT,
  TURN_RIGHT,
  STOP
}ControlCommand;

CREATE_TASK(Car)
uint8_t initSpeedMotorLeft, initSpeedMotorRight;
uint8_t enable1LeftPin, enable1RightPin;
uint8_t enable2LeftPin, enable2RightPin;
uint8_t controlSpeedLeft, controlSpeedRight;

uint8_t leftSensorPin, midSensorPin, rightSensorPin;

ControlCommand cmd = STOP;

enumStateCar previousSate;

HCSR04 *ultrasonic;
Servo *servo;

void loop();
void start();
void stop();

void setupMotor();
void setupMotorLeft(uint8_t enable1LeftPin, uint8_t enable2LeftPin, uint8_t controlSpeedLeft, uint8_t initSpeedMotorLeft);
void setupMotorRight(uint8_t enable1RightPin, uint8_t enable2RightPin, uint8_t controlSpeedRight, uint8_t initSpeedMotorRight);

void setupLineSensor();
void setupLineSensor(uint8_t leftSensorPin, uint8_t rightSensorPin);
void setupLineSensor(uint8_t leftSensorPin, uint8_t midSensorPin, uint8_t rightSensorPin);

void setupUltrasonic(uint8_t ultrasonicTricPin, uint8_t ultrasonicEchoPin, HCSR04 *ultrasonic);

void setupServo(uint8_t servoPin, Servo *servo);
void setPreviousState(enumStateCar state);
void setCommand(ControlCommand cmd);

ControlCommand getCommand();
enumStateCar getPreviousState();
void goForward();
void goBackward();
void turnLeft();
void turnRight();
void carStop();

void voiceMode();
void followLineMode();
void avoidObstacleMode();



END

#endif