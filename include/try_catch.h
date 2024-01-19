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

#define TRY(aim, buffer, value)    if ((aim = setjmp(buffer)) == (value)) 
#define CATCH(aim , value)  else if (aim == (value)) 
#define THROW(buffer, value)  longjmp(buffer, (value));

#endif