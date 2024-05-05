#ifndef __VOICE_H
#define __VOICE_H

#include "kxnTask.h"
#include "debug.h"
#include "my_car.h"

//#define DEBUG

#define CONTROL_COMMAND 0x08

DEFINE_TASK_STATE(Voice){
  NOTHING
};

typedef enum{
  MODE_DEFAULT,
  MODE_1,
  MODE_2,
  MODE_3
}ModeCommand;

CREATE_TASK(Voice)

Car *myCar;

ModeCommand mode;

//void setup();
void setup(int baudRate);
//void loop();
void runFast();
void start();
void stop();

void setModeCommand(ModeCommand mode);

void setBaudRate(int baudRate);

void setVoiceControl(Car* car);

ModeCommand getModeCommand();


END
#endif