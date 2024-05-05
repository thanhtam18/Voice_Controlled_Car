#include "my_car.h"
#include "my_voice.h"
#include "my_HCSR04.h"
#include "Servo.h"

Servo myServo;
HCSR04  myHCSR04;
Voice myVoice;
Car myCar;

void setup() {
  // put your setup code here, to run once:
  myVoice.setup(9600);
  myVoice.setVoiceControl(&myCar);
  myVoice.setModeCommand(MODE_1);

  myCar.setupMotor();
  myCar.setupUltrasonic(2, 3, &myHCSR04);
  myCar.setupServo(10, &myServo);

  myVoice.start();
  myCar.start();
}

void loop() {
  // put your main code here, to run repeatedly:
  kxnTaskManager.run(millis());
}
