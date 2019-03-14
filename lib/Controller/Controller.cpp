#ifndef Controller_H
#define Controller_H

#include "TimedAction.h"
#include <Homie.h>

using namespace std;

class Controller {
protected:
  String name;
  int conf[20];

public:
  Controller(const int id) : name(id) {
    timer =
        new TimedAction(1 * 60 * 1000, bind(&Controller::updateStatus, this));
  }
  virtual void loop() { timer->check(); }
  virtual void updateStatus() = 0;

  void log(String logger, String text) {
    Homie.getLogger() << "[" << logger << "] " << (const char *)text.c_str()
                      << endl;
  }

protected:
  TimedAction *timer;
};

#endif
