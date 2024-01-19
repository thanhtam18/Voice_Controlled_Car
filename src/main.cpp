#include <Arduino.h>
#include <main_file.h>
#include <try_catch.h>
#include <motor_control.h>
#include <Servo.h>


jmp_buf buf;
uint8_t command;

uint8_t modeControl = MODE_TWO;
Car myCar;



void setup() {
  Serial.begin(9600);
  GPIO_Init();
  myCar.myCarInit(150,190);
  delay(1000);
}


void loop() {
  if(Serial.available()){
    uint8_t receiveHexValue = Serial.read();
    if((receiveHexValue & 0x08) && !(modeControl - MODE_ONE)){
      THROW(buf, (Command)receiveHexValue);
    }
    switch(receiveHexValue){
      case 0x01:
        ledToggle(1);
        modeControl = MODE_ONE;
        break;
      case 0x02:
        ledToggle(2);
        modeControl = MODE_TWO;
        break;
      case 0x03:
        ledToggle(3);
        modeControl = MODE_THREE;
        break;
    }
  }
  if(modeControl == MODE_ONE){
    TRY(command, buf, FORWARD)
      myCar.goForward();
    CATCH(command, BACKWARD)
      myCar.goBackward();
    CATCH(command, TURN_LEFT)
      myCar.turnLeft();
    CATCH(command, TURN_RIGHT)
      myCar.turnRight();
    CATCH(command, STOP)
      myCar.stop();
  }
  else if(modeControl == MODE_TWO){
    myCar.lineFollower();
  }
  else if(modeControl == MODE_THREE){
    myCar.obstacleAvoiding();
  }
}

void GPIO_Init(){
  pinMode(LINE_SENSOR_LEFT, INPUT);
  pinMode(LINE_SENSOR_RIGHT, INPUT);
  pinMode(CONTROL_SPEED_MOTOR_LEFT, OUTPUT);
  pinMode(ENABLE1_MOTOR_LEFT, OUTPUT);
  pinMode(ENABLE2_MOTOR_LEFT, OUTPUT);
  pinMode(CONTROL_SPEED_MOTOR_RIGHT, OUTPUT);
  pinMode(ENABLE1_MOTOR_RIGHT, OUTPUT);
  pinMode(ENABLE2_MOTOR_RIGHT, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void ledToggle(uint8_t times){
  while(times-- > 0){
    digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
    delay(100);
    digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
    delay(100);
  }

}