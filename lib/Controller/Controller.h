#ifndef Controller_H
#define Controller_H

#include "OneButton.h"
#include "Light.h"
#include "LightNode.h"
#include "TimedAction.h"
#include <vector>
#include <string>

using namespace std;

class Controller {
protected:
    String name;
    int conf[20];
  public:
    Controller(const String& id);
    virtual void loop();
};

class ControllerFactory {
  public:
    ControllerFactory();
    Controller* createController(const String& id, const char* conf);

  private:
    void split(const string &s, const char* delim, vector<string> &v);
};

#endif
