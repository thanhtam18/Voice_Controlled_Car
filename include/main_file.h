/*
    File:   main_file.h
    Author: Thanh Tam
    Date:   13/1/2024
*/
#ifndef _MAIN_FILE_H
#define _MAIN_FILE_H

#include "setjmp.h"
#include <stdint.h>

#define TRY(aim, buffer, value)    if ((aim = setjmp(buffer)) == (value)) 
#define CATCH(aim , value)  else if (aim == (value)) 
#define THROW(buffer, value)  longjmp(buffer, (value));

typedef enum{
  MODE_DEFAULT,   
  MODE_ONE,   
  MODE_TWO,    
  MODE_THREE     
}ModeControl;

typedef enum{
  FORWARD = 0x0A,
  BACKWARD,
  TURN_LEFT,
  TURN_RIGHT,
  STOP
}Command;

void GPIO_Init();

#endif