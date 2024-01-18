#include <motor_control.h>

/*
    Function: speedMotorInit
    Description: This function set PWM for motor left and right

    Input: 
        - speedMotorLeft: the speed of motor left range 0 to 255
        - speedMotorRight: the speed of motor right range 0 to 255

    Ouput: None
*/
void Car :: speedMotorInit(uint8_t speedMotorLeft, uint8_t speedMotorRight){
    this->speedMotorLeftInit = speedMotorLeft;
    this->speedMotorRightInit = speedMotorRight;
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
    delay(1000);
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
    CONTROL_SPEED(CONTROL_SPEED_MOTOR_LEFT, 240);
    CONTROL_SPEED(CONTROL_SPEED_MOTOR_RIGHT, 250);
    ENABLE_MOTOR(ENABLE1_MOTOR_LEFT, HIGH);
    ENABLE_MOTOR(ENABLE2_MOTOR_LEFT, LOW);
    ENABLE_MOTOR(ENABLE1_MOTOR_RIGHT, HIGH);
    ENABLE_MOTOR(ENABLE2_MOTOR_RIGHT, LOW);
    delay(1000);
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
    CONTROL_SPEED(CONTROL_SPEED_MOTOR_LEFT, 0);
    CONTROL_SPEED(CONTROL_SPEED_MOTOR_RIGHT, 0);
    ENABLE_MOTOR(ENABLE1_MOTOR_LEFT, LOW);
    ENABLE_MOTOR(ENABLE2_MOTOR_LEFT, LOW);
    ENABLE_MOTOR(ENABLE1_MOTOR_RIGHT, LOW);
    ENABLE_MOTOR(ENABLE2_MOTOR_RIGHT, LOW);
}

/*
    Function: turnLeft
    Description: This function will make follow the line
    Input: None
    Ouput: None
*/
void Car :: lineFollower(){
    ENABLE_MOTOR(ENABLE1_MOTOR_LEFT, HIGH);
    ENABLE_MOTOR(ENABLE2_MOTOR_LEFT, LOW);
    ENABLE_MOTOR(ENABLE1_MOTOR_RIGHT, LOW);
    ENABLE_MOTOR(ENABLE2_MOTOR_RIGHT, HIGH);
    double PID;
    int8_t lineDetected;
    while((lineDetected = lineDetection()) != STOP_LINE){
        PID = calculatePID(lineDetected);
        CONTROL_SPEED(CONTROL_SPEED_MOTOR_LEFT, constrain((speedMotorLeftInit + PID),0, 255));
        CONTROL_SPEED(CONTROL_SPEED_MOTOR_RIGHT, constrain((speedMotorRightInit - PID),0, 255));
        HANDLE_UART(STOP, stop, &modeControl);
    } 
    stop();
}

static HCSR04 ultrasonicSensor(ULTRASONIC_TRIG, ULTRASONIC_ECHO);

void Car :: obstacleAvoiding(){
    float distance = ultrasonicSensor.dist();
    uint8_t index = 0;
    while(distance <= 15.00){
        stop();
        delay(500);
        switch(index){
            case 0:
                turnLeft();
                break;
            case 1:
                turnRight();
                turnRight();
                break;
            case 2:
                turnRight();
                index = 0;
                break;
        }
        index++;
        distance = ultrasonicSensor.dist();
        HANDLE_UART(STOP, stop, &modeControl);
    }
    goForward();
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
    if(leftSensorAnalog > 400)
        return LEFT;
    else if(rightSensorAnalog > 400)
        return RIGHT;
    else    
        return STOP_LINE;
    return MID;
}

/*
    Function: calculatePID
    Description: Calculate PID value
    Input: Error value after detected line 
    Ouput: PID value
*/
double Car :: calculatePID(int8_t error){
    register uint8_t l_Kp = this->Kp;
    register uint8_t l_Ki = this->Ki;
    register uint8_t l_Kd = this->Kd;
    static int8_t lastError;
    static int8_t integral;
    integral += error;
    int8_t derivative = error - lastError;
    lastError = error;
    return (l_Kp * error + l_Ki * integral + l_Kd * derivative);
}