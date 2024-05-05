#ifndef __DEBUG_H
#define __DEBUG_H

#define LOG(cmd) #cmd

#define PRINT_DEBUG(s)  Serial.println(LOG(s));


#endif  