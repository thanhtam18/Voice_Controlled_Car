#include <motor_control.h>

static Servo myServo;
static HCSR04 ultrasonicSensor(ULTRASONIC_TRIG, ULTRASONIC_ECHO);

/*
    Function: speedMotorInit
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
    Function: goForward
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
    Function: goBackward
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
    Function: turnLeft
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
    Function: turnRight
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
    Function: stop
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

void _break(){
    CONTROL_SPEED(CONTROL_SPEED_MOTOR_LEFT, 255);
    CONTROL_SPEED(CONTROL_SPEED_MOTOR_RIGHT, 255);
    ENABLE_MOTOR(ENABLE1_MOTOR_LEFT, LOW);
    ENABLE_MOTOR(ENABLE2_MOTOR_LEFT, HIGH);
    ENABLE_MOTOR(ENABLE1_MOTOR_RIGHT, HIGH);
    ENABLE_MOTOR(ENABLE2_MOTOR_RIGHT, LOW);
    _delay_us(700);
    ENABLE_MOTOR(ENABLE1_MOTOR_LEFT, HIGH);
    ENABLE_MOTOR(ENABLE2_MOTOR_LEFT, HIGH);
    ENABLE_MOTOR(ENABLE1_MOTOR_RIGHT, HIGH);
    ENABLE_MOTOR(ENABLE2_MOTOR_RIGHT, HIGH);
    CONTROL_SPEED(CONTROL_SPEED_MOTOR_LEFT, 0);
    CONTROL_SPEED(CONTROL_SPEED_MOTOR_RIGHT, 0);
}

/*
    Function: turnLeft
    Description: This function will make follow the line
    Input: None
    Ouput: None
*/
void Car :: lineFollower(){
    int8_t lineDetected;
    while((lineDetected = lineDetection()) != DETECTED_STOP_LINE){
        switch(lineDetected){
            case DETECTED_LEFT:
                _break();
                while ((lineDetected = lineDetection()) != DETECTED_MID && (lineDetected = lineDetection()) != DETECTED_STOP_LINE)
                {                                   
                    ENABLE_MOTOR(ENABLE1_MOTOR_LEFT, LOW);
                    ENABLE_MOTOR(ENABLE2_MOTOR_LEFT, HIGH);
                    ENABLE_MOTOR(ENABLE1_MOTOR_RIGHT, LOW);
                    ENABLE_MOTOR(ENABLE2_MOTOR_RIGHT, HIGH);
                    CONTROL_SPEED(CONTROL_SPEED_MOTOR_LEFT, 180);
                    CONTROL_SPEED(CONTROL_SPEED_MOTOR_RIGHT, 255);
                }
                stop();
                break;
            case DETECTED_RIGHT:
                _break();
                while ((lineDetected = lineDetection()) != DETECTED_MID && (lineDetected = lineDetection()) != DETECTED_STOP_LINE)
                {                                   
                    ENABLE_MOTOR(ENABLE1_MOTOR_LEFT, HIGH);
                    ENABLE_MOTOR(ENABLE2_MOTOR_LEFT, LOW);
                    ENABLE_MOTOR(ENABLE1_MOTOR_RIGHT, HIGH);
                    ENABLE_MOTOR(ENABLE2_MOTOR_RIGHT, LOW);             
                    CONTROL_SPEED(CONTROL_SPEED_MOTOR_LEFT, 255);
                    CONTROL_SPEED(CONTROL_SPEED_MOTOR_RIGHT, 180);
                }               
                stop();
                break;
            case DETECTED_MID:
                CONTROL_SPEED(CONTROL_SPEED_MOTOR_LEFT, 0);
                CONTROL_SPEED(CONTROL_SPEED_MOTOR_RIGHT, 0);
                ENABLE_MOTOR(ENABLE1_MOTOR_LEFT, HIGH);
                ENABLE_MOTOR(ENABLE2_MOTOR_LEFT, LOW);
                ENABLE_MOTOR(ENABLE1_MOTOR_RIGHT, LOW);
                ENABLE_MOTOR(ENABLE2_MOTOR_RIGHT, HIGH);
                for(int i = 100; (lineDetected = lineDetection()) == DETECTED_MID && i < 161; i++){
                    CONTROL_SPEED(CONTROL_SPEED_MOTOR_LEFT, i);
                    CONTROL_SPEED(CONTROL_SPEED_MOTOR_RIGHT, i);
                    delay(5);
                }
                break;
        }
        HANDLE_UART(STOP, stop, &modeControl);
    } 
    stop();
    HANDLE_UART(STOP, stop, &modeControl);
}



void Car :: obstacleAvoiding(){
    
    float distance = ultrasonicSensor.dist();
    while(distance > 15.0){
        goForward();
        distance = ultrasonicSensor.dist();
        delay(60);
        HANDLE_UART(STOP, stop, &modeControl);
    }
    bool isAnotherWay = false;
        stop();
        uint8_t index = 1;
        while(!isAnotherWay){
            switch(index){
                case 1:
                    index++;
                    myServo.write(180);
                    delay(500);
                    distance = ultrasonicSensor.dist();
                    myServo.write(90);
                    delay(500);
                    if(distance > 15.0){
                        turnLeft();
                        index = 0;
                        isAnotherWay = true;
                    }
                    break;
                case 2:
                    index++;
                    myServo.write(0);
                    delay(500);
                    distance = ultrasonicSensor.dist();
                    myServo.write(90);
                    delay(500);
                    if(distance > 15.0){
                        turnRight();
                        index = 0;
                        isAnotherWay = true;
                    }
                    break;
                case 3:
                    goBackward();
                    delay(500);
                    stop();
                    index = 1;
                    break;
            }
            HANDLE_UART(STOP, stop, &modeControl);
        }
        
    }

/*
    Function: lineDetection
    Description: Read line sensor
    Input: None
    Ouput: Error value
*/
int8_t Car :: lineDetection(){
    uint16_t leftSensorAnalog = READ_SENSOR_LEFT;
    uint16_t rightSensorAnalog = READ_SENSOR_RIGHT;
    if(leftSensorAnalog > 500 && rightSensorAnalog < 500)
        return DETECTED_LEFT;
    else if(rightSensorAnalog > 500 && leftSensorAnalog < 500)
        return DETECTED_RIGHT;
    else if (rightSensorAnalog > 500 && leftSensorAnalog > 500)
        return DETECTED_STOP_LINE;
    return DETECTED_MID;
}
