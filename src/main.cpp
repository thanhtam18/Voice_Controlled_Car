#include <Arduino.h>
#include <main_file.h>
#include <try_catch.h>
#include <motor_control.h>

uint8_t modeControl = MODE_DEFAULT;
Car myCar;

void setup() {
  Serial.begin(9600);
  GPIO_Init();
  myCar.speedMotorInit(145,150);
}

void loop() {
  if(Serial.available()){
    uint8_t receiveHexValue = Serial.read();
    if((receiveHexValue & 0x08) && !(modeControl - MODE_ONE)){
      THROW((Command)receiveHexValue);
    }
    switch(receiveHexValue){
      case 0x01:
        modeControl = MODE_ONE;
        break;
      case 0x02:
        modeControl = MODE_TWO;
        break;
      case 0x03:
        modeControl = MODE_THREE;
        break;
    }
  }
  if(modeControl == MODE_ONE){
    Serial.println("Mode 1 Enable");
    TRY(FORWARD)
      myCar.goForward();
    CATCH(BACKWARD)
      myCar.goBackward();
    CATCH(TURN_LEFT)
      myCar.turnLeft();
    CATCH(TURN_RIGHT)
      myCar.turnRight();
    CATCH(STOP)
      myCar.stop();
  }
  else if(modeControl == MODE_TWO){
    myCar.lineFollower();
  }
  else if(modeControl == MODE_THREE){
    myCar.obstacleAvoiding();
  }
  delay(500);
}

void GPIO_Init(){
  pinMode(CONTROL_SPEED_MOTOR_LEFT, OUTPUT);
  pinMode(ENABLE1_MOTOR_LEFT, OUTPUT);
  pinMode(ENABLE2_MOTOR_LEFT, OUTPUT);
  pinMode(CONTROL_SPEED_MOTOR_RIGHT, OUTPUT);
  pinMode(ENABLE1_MOTOR_RIGHT, OUTPUT);
  pinMode(ENABLE2_MOTOR_RIGHT, OUTPUT);
}