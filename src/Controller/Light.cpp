#ifndef Light_H
#define Light_H

#include <Arduino.h>

class Light {
  int _pin;

public:
  Light(int pin) {
  this->_pin = pin;
  pinMode(pin, OUTPUT);
}

  void on() { digitalWrite(_pin, HIGH); }

  void off() { digitalWrite(_pin, LOW); }

  bool isActive() { return digitalRead(_pin) == HIGH ? true : false; }
};

#endif
