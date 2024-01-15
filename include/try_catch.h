/*
    File:   try_cath.h
    Author: Thanh Tam
    Date:   13/1/2024
    Description: Using macro to make a long jump
*/
#ifndef _TRY_CATCH_H
#define _TRY_CATCH_H

#include "setjmp.h"
#include <stdint.h>

jmp_buf buf;
uint8_t command;
#define TRY(x)    if ((command = setjmp(buf)) == (x)) 
#define CATCH(x)  else if (command == (x)) 
#define THROW(x)  longjmp(buf, (x));

#endif