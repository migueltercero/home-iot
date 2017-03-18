#ifndef LightController_H
#define LightController_H

#include "Controller.h"
#include "LightNode.h"
#include "OneButton.h"
#include "Light.h"


using namespace std;
using namespace std::placeholders;

class LightController : public Controller{
  protected:
    Light *light;
    LightNode *mqtt;
    OneButton *button;


    void buttonClickHandler();

    void mqttStatusHandler(bool active);

    void off();

    void on();

  public:
    LightController(const String& id, const int buttonPin, const int lightPin);
    void loop();
};
#endif
