#ifndef __HCSR04_H
#define __HCSR04_H

#include "Arduino.h"

class HCSR04 {
private:
  uint8_t echoPin, tricPin;
public:
  void setupPin(uint8_t echoPin, uint8_t tricPin);
  float dist() const;
};


#endif