#include "Light.h"
#include <Arduino.h>

Light::Light(int pin) {
  this->_pin = pin;
  pinMode(pin, OUTPUT);
}

void Light::on() { digitalWrite(_pin, HIGH); }

void Light::off() { digitalWrite(_pin, LOW); }

bool Light::isActive() { return digitalRead(_pin) == HIGH ? true : false; }

