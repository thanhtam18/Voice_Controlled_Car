#include <motor_control.h>

static Servo myServo;
static HCSR04 ultrasonicSensor(ULTRASONIC_TRIG, ULTRASONIC_ECHO);

/*
    Description: This function set PWM for motor left and right
    Input: 
        - speedMotorLeft: the speed of motor left range 0 to 255
        - speedMotorRight: the speed of motor right range 0 to 255

    Ouput: None
*/
void Car :: myCarInit(uint8_t speedMotorLeft, uint8_t speedMotorRight){
    this->speedMotorLeftInit = speedMotorLeft;
    this->speedMotorRightInit = speedMotorRight;
    myServo.attach(CONTROL_SERVO);
    myServo.write(90);
}

/*
    Description: This function will make the car go forward
    Input: None
    Ouput: None
*/
void Car :: goForward(){
    CONTROL_SPEED(CONTROL_SPEED_MOTOR_LEFT, speedMotorLeftInit);
    CONTROL_SPEED(CONTROL_SPEED_MOTOR_RIGHT, speedMotorRightInit);
    ENABLE_MOTOR(ENABLE1_MOTOR_LEFT, HIGH);
    ENABLE_MOTOR(ENABLE2_MOTOR_LEFT, LOW);
    ENABLE_MOTOR(ENABLE1_MOTOR_RIGHT, LOW);
    ENABLE_MOTOR(ENABLE2_MOTOR_RIGHT, HIGH);
}

/*
    Description: This function will make the car go backward
    Input: None
    Ouput: None
*/
void Car :: goBackward(){
    CONTROL_SPEED(CONTROL_SPEED_MOTOR_LEFT, speedMotorLeftInit);
    CONTROL_SPEED(CONTROL_SPEED_MOTOR_RIGHT, speedMotorRightInit);
    ENABLE_MOTOR(ENABLE1_MOTOR_LEFT, LOW);
    ENABLE_MOTOR(ENABLE2_MOTOR_LEFT, HIGH);
    ENABLE_MOTOR(ENABLE1_MOTOR_RIGHT, HIGH);
    ENABLE_MOTOR(ENABLE2_MOTOR_RIGHT, LOW);
}

/*
    Description: This function will make the car turn left
    Input: None
    Ouput: None
*/
void Car :: turnLeft(){
    CONTROL_SPEED(CONTROL_SPEED_MOTOR_LEFT, 250);
    CONTROL_SPEED(CONTROL_SPEED_MOTOR_RIGHT, 250);
    ENABLE_MOTOR(ENABLE1_MOTOR_LEFT, LOW);
    ENABLE_MOTOR(ENABLE2_MOTOR_LEFT, HIGH);
    ENABLE_MOTOR(ENABLE1_MOTOR_RIGHT, LOW);
    ENABLE_MOTOR(ENABLE2_MOTOR_RIGHT, HIGH);
    delay(600);
    ENABLE_MOTOR(ENABLE2_MOTOR_LEFT, LOW);
    ENABLE_MOTOR(ENABLE2_MOTOR_RIGHT, LOW);
}

/*
    Description: This function will make the car turn right
    Input: None
    Ouput: None
*/
void Car :: turnRight(){
    CONTROL_SPEED(CONTROL_SPEED_MOTOR_LEFT, 250);
    CONTROL_SPEED(CONTROL_SPEED_MOTOR_RIGHT, 250);
    ENABLE_MOTOR(ENABLE1_MOTOR_LEFT, HIGH);
    ENABLE_MOTOR(ENABLE2_MOTOR_LEFT, LOW);
    ENABLE_MOTOR(ENABLE1_MOTOR_RIGHT, HIGH);
    ENABLE_MOTOR(ENABLE2_MOTOR_RIGHT, LOW);
    delay(600);
    ENABLE_MOTOR(ENABLE1_MOTOR_LEFT, LOW);
    ENABLE_MOTOR(ENABLE1_MOTOR_RIGHT, LOW);
}

/*
    Description: This function will make the car stop
    Input: None
    Ouput: None
*/
void Car :: stop(){
    CONTROL_SPEED(CONTROL_SPEED_MOTOR_LEFT, 255);
    CONTROL_SPEED(CONTROL_SPEED_MOTOR_RIGHT, 255);
    ENABLE_MOTOR(ENABLE1_MOTOR_LEFT, HIGH);
    ENABLE_MOTOR(ENABLE2_MOTOR_LEFT, HIGH);
    ENABLE_MOTOR(ENABLE1_MOTOR_RIGHT, HIGH);
    ENABLE_MOTOR(ENABLE2_MOTOR_RIGHT, HIGH);
    CONTROL_SPEED(CONTROL_SPEED_MOTOR_LEFT, 0);
    CONTROL_SPEED(CONTROL_SPEED_MOTOR_RIGHT, 0);
}
#if defined(PID_MODE)
float P, D, I, PID_value;
int error, pre_error;

void Car :: PID(){
  P = error;
  D = error - pre_error;
  I += error;
  PID_value = Kp*P + Ki*I + Kd*D;
  pre_error = error;
}

/*
    Function: lineDetection
    Description: Read line sensor
    Input: None
    Ouput: Error value
*/

static int lineDetection(){
    uint16_t leftSensorAnalog = READ_SENSOR_LEFT;
    uint16_t midSensorAnalog = READ_SENSOR_MID;
    uint16_t rightSensorAnalog = READ_SENSOR_RIGHT;
    //Serial.println(String(leftSensorAnalog) + "  " + String(midSensorAnalog) + "   " + String(rightSensorAnalog));
    static LineDetect lastDetected;
    if(leftSensorAnalog > DETECTED && rightSensorAnalog < DETECTED && (midSensorAnalog < DETECTED || midSensorAnalog >DETECTED)){
      lastDetected = DETECTED_LEFT;
      return DETECTED_LEFT;
    }
    else if(leftSensorAnalog < DETECTED && midSensorAnalog > DETECTED && rightSensorAnalog < DETECTED){
      lastDetected = DETECTED_MID;
      return DETECTED_MID;
    }
    else if(leftSensorAnalog < DETECTED && rightSensorAnalog > DETECTED && (midSensorAnalog < DETECTED || midSensorAnalog >DETECTED)){
      lastDetected = DETECTED_RIGHT;
      return DETECTED_RIGHT;
    }
    else if(leftSensorAnalog > DETECTED && midSensorAnalog > DETECTED && rightSensorAnalog > DETECTED){
      lastDetected = DETECTED_STOP_LINE;
      return DETECTED_STOP_LINE;
    }
    else
      return lastDetected;
}
#else
void Car :: forward(){
    CONTROL_SPEED(CONTROL_SPEED_MOTOR_LEFT, 150);
    CONTROL_SPEED(CONTROL_SPEED_MOTOR_RIGHT, 150);
    ENABLE_MOTOR(ENABLE1_MOTOR_LEFT, HIGH);
    ENABLE_MOTOR(ENABLE2_MOTOR_LEFT, LOW);
    ENABLE_MOTOR(ENABLE1_MOTOR_RIGHT, LOW);
    ENABLE_MOTOR(ENABLE2_MOTOR_RIGHT, HIGH);
}
void Car :: left(){
    CONTROL_SPEED(CONTROL_SPEED_MOTOR_LEFT, 190);
    CONTROL_SPEED(CONTROL_SPEED_MOTOR_RIGHT, 190);
    ENABLE_MOTOR(ENABLE1_MOTOR_LEFT, LOW);
    ENABLE_MOTOR(ENABLE2_MOTOR_LEFT, HIGH);
    ENABLE_MOTOR(ENABLE1_MOTOR_RIGHT, LOW);
    ENABLE_MOTOR(ENABLE2_MOTOR_RIGHT, HIGH);
}
void Car :: right(){
    CONTROL_SPEED(CONTROL_SPEED_MOTOR_LEFT, 190);
    CONTROL_SPEED(CONTROL_SPEED_MOTOR_RIGHT, 190);
    ENABLE_MOTOR(ENABLE1_MOTOR_LEFT, HIGH);
    ENABLE_MOTOR(ENABLE2_MOTOR_LEFT, LOW);
    ENABLE_MOTOR(ENABLE1_MOTOR_RIGHT, HIGH);
    ENABLE_MOTOR(ENABLE2_MOTOR_RIGHT, LOW);
}
#endif

/*
    Description: This function will make follow the line
    Input: None
    Ouput: None
*/
void Car :: lineFollower(){
    #if defined(PID_MODE)
    error = lineDetection();
    PID();
    if(error != DETECTED_STOP_LINE){
        int leftMotorSpeed = 150 + PID_value;
        int rightMotorSpeed = 150 - PID_value;
        if(leftMotorSpeed < 0){
            CONTROL_SPEED(CONTROL_SPEED_MOTOR_LEFT, -1*(constrain(leftMotorSpeed, -190, 0)));
            ENABLE_MOTOR(ENABLE1_MOTOR_LEFT, LOW);
            ENABLE_MOTOR(ENABLE2_MOTOR_LEFT, HIGH);  
        }
        else{
            CONTROL_SPEED(CONTROL_SPEED_MOTOR_LEFT, constrain(leftMotorSpeed, 0, 255));
            ENABLE_MOTOR(ENABLE1_MOTOR_LEFT, HIGH);
            ENABLE_MOTOR(ENABLE2_MOTOR_LEFT, LOW);
        }
        if(rightMotorSpeed < 0){
            CONTROL_SPEED(CONTROL_SPEED_MOTOR_RIGHT, -1*(constrain(rightMotorSpeed, -190, 0)));
            ENABLE_MOTOR(ENABLE1_MOTOR_RIGHT, HIGH);
            ENABLE_MOTOR(ENABLE2_MOTOR_RIGHT, LOW);
        } 
        else{
            CONTROL_SPEED(CONTROL_SPEED_MOTOR_RIGHT, constrain(rightMotorSpeed, 0, 255));
            ENABLE_MOTOR(ENABLE1_MOTOR_RIGHT, LOW);
            ENABLE_MOTOR(ENABLE2_MOTOR_RIGHT, HIGH);    
        }
    }
    else{
        stop();
    }
    #else
        uint16_t leftSensorAnalog = READ_SENSOR_LEFT;
        uint16_t midSensorAnalog = READ_SENSOR_MID;
        uint16_t rightSensorAnalog = READ_SENSOR_RIGHT;
        float distance = 0;
        static unsigned long previous = millis();
        if(millis() - previous > 60){
            distance= ultrasonicSensor.dist();
            previous = millis();
        }
        if(distance > 20){
            if(leftSensorAnalog > DETECTED && rightSensorAnalog < DETECTED && (midSensorAnalog < DETECTED || midSensorAnalog >DETECTED)){
                left();
            }
            else if(leftSensorAnalog < DETECTED && midSensorAnalog > DETECTED && rightSensorAnalog < DETECTED){
                forward();
            }
            else if(leftSensorAnalog < DETECTED && rightSensorAnalog > DETECTED && (midSensorAnalog < DETECTED || midSensorAnalog >DETECTED)){
                right();
            }
            else if(leftSensorAnalog > DETECTED && midSensorAnalog > DETECTED && rightSensorAnalog > DETECTED){
                stop();
            }
        }
        else{
            stop();
        }
    #endif
}


void Car :: obstacleAvoiding(){
    delay(60);
    static bool isAnotherWay = true;
    float distance = ultrasonicSensor.dist();
    if(distance > 20.0 && isAnotherWay){
        goForward();
    }
    else{
        isAnotherWay = false;
        stop();
        static uint8_t index = 1;
        switch(index){
                case 1:
                    myServo.write(180);
                    delay(500);
                    distance = ultrasonicSensor.dist();                
                    myServo.write(90);   
                    delay(500);     
                    if(distance > 20.0){
                        turnLeft();
                        isAnotherWay = true;
                        index = 1;
                    }
                    else
                        index++;
                    break;
                case 2:
                    myServo.write(0);
                    delay(500);
                    distance = ultrasonicSensor.dist();
                    myServo.write(90);
                    delay(500);
                    if(distance > 20.0){
                        turnRight();
                        isAnotherWay = true;
                        index = 1;
                    }
                    else
                        index++;
                    break;
                case 3:
                    goBackward();
                    delay(500);
                    stop();
                    index = 1;
                    break;
        }
    }
}