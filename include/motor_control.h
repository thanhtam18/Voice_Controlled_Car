/*
    File:   motor_control.h
    Author: Thanh Tam
    Date:   13/1/2024
    Description: Control the speed of the car
*/
#ifndef _MOTOR_CONTROL_H
#define _MOTOR_CONTROL_H

#include <stdint.h>
#include <Arduino.h>

#define CONTROL_SPEED_MOTOR_LEFT    6
#define ENABLE1_MOTOR_LEFT          8
#define ENABLE2_MOTOR_LEFT          9    

#define CONTROL_SPEED_MOTOR_RIGHT   5
#define ENABLE1_MOTOR_RIGHT         4
#define ENABLE2_MOTOR_RIGHT         7

#define CONTROL_SPEED(controlSpeed, value)       analogWrite(controlSpeed, value);
#define ENABLE_MOTOR(enableMotor, level)         digitalWrite(enableMotor, level);


class Car{
    private:
        uint8_t speedMotorLeftInit;
        uint8_t speedMotorRightInit;
    public:
        void speedMotorInit(uint8_t speedMotorLeft, uint8_t speedMotorRight);
        void goForward();
        void goBackward();
        void turnLeft();
        void turnRight();
        void stop();
        void lineFollower();
        void obstacleAvoiding();
};

#endif