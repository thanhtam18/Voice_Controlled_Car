#include "my_HCSR04.h"

void HCSR04::setupPin(uint8_t echoPin, uint8_t tricPin) {
  this->echoPin = echoPin;
  this->tricPin = tricPin;
  pinMode(this->echoPin, INPUT);
  pinMode(this->tricPin, OUTPUT);
}

float HCSR04::dist() const {
  digitalWrite(this->tricPin, LOW);
  delayMicroseconds(2);
  digitalWrite(this->tricPin, HIGH);
  delayMicroseconds(10);
  noInterrupts();
  digitalWrite(this->tricPin, LOW);
  float d = pulseIn(this->echoPin, HIGH, 23529.4);
  interrupts();
  return d / 58.8235;
}
