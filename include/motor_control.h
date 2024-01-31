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
#include <main_file.h>
#include <HCSR04.h>
#include <Servo.h>

#define CONTROL_SPEED_MOTOR_LEFT    6
#define ENABLE1_MOTOR_LEFT          8
#define ENABLE2_MOTOR_LEFT          9    

#define CONTROL_SPEED_MOTOR_RIGHT   5
#define ENABLE1_MOTOR_RIGHT         4
#define ENABLE2_MOTOR_RIGHT         7

#define CONTROL_SPEED(controlSpeed, value)       analogWrite(controlSpeed, value);
#define ENABLE_MOTOR(enableMotor, level)         digitalWrite(enableMotor, level);

#define LINE_SENSOR_LEFT            A1
#define LINE_SENSOR_MID             A2
#define LINE_SENSOR_RIGHT           A3

#define READ_SENSOR_LEFT                         analogRead(LINE_SENSOR_LEFT);
#define READ_SENSOR_MID                          analogRead(LINE_SENSOR_MID);
#define READ_SENSOR_RIGHT                        analogRead(LINE_SENSOR_RIGHT);

#define DETECTED            400

#define CONTROL_SERVO               11

#define HANDLE_UART(condition, action, modeReturn)  if(Serial.available()){\
                                                        if(Serial.read() == condition){\
                                                            action();\
                                                            uint8_t *ptr = modeReturn;\
                                                            *ptr = MODE_DEFAULT;\
                                                            return;\
                                                        }\
                                                    }



typedef enum{
    DETECTED_LEFT = -2,
    DETECTED__HALF_LEFT,
    DETECTED_MID,
    DETECTED__HALF_RIGHT,
    DETECTED_RIGHT,
    DETECTED_STOP_LINE
}LineDetect;

extern uint8_t modeControl;

class Car{
    private:
        uint8_t speedMotorLeftInit;
        uint8_t speedMotorRightInit;
        float Kp , Kd , Ki;
        void PID();
        void forward();
        void left();
        void right();
    public:
        Car(){Kp = 170, Ki = 0.1, Kd = 100;};
        void myCarInit(uint8_t speedMotorLeft, uint8_t speedMotorRight);
        void goForward();
        void goBackward();
        void turnLeft();
        void turnRight();
        void stop();
        void lineFollower();
        void obstacleAvoiding();
};

#endif