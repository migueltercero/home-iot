#ifndef Logger_H
#define Logger_H

#include <Homie.h>
#include <typeinfo>
#include "TimeLib.h"

template <class T> class Logger {
public:
  void info(String text) {
    Homie.getLogger() << "|" << getFormattedTime() << "|" << typeid(T).name() << "|" << (const char*)text.c_str() << endl;
  }
  String getFormattedTime() {
    char formatedTime[20];
    sprintf(formatedTime, "%i/%i/%i %i:%i:%i", day(), month(), year(), hour(), minute(), second());
    return String(formatedTime);
  }
};

#endif