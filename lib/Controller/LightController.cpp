#ifndef LightController_H
#define LightController_H

#include "Controller.cpp"
#include "Light.h"
#include "LightNode.cpp"
#include "OneButton.h"

using namespace std;
using namespace std::placeholders;

class LightController : public Controller {
public:
  LightController(const int id, const int buttonPin, const int lightPin)
      : Controller(id) {
    light = new Light(lightPin);
    mqtt = new LightNode(this->name);
    button = new OneButton(buttonPin, true);

    button->attachClick(bind(&LightController::buttonClickHandler, this));
    mqtt->attachProperty(bind(&LightController::mqttStatusHandler, this, _1));
  }

  void updateStatus() {
    boolean status = light->isActive();
    mqtt->send(status);
    log("LightController", "mqtt update: " + mqtt->toString(status));
  }

  void loop() {
    Controller::loop();
    button->tick();
  }

protected:
  Light *light;
  LightNode *mqtt;
  OneButton *button;

  void buttonClickHandler() {
    if (light->isActive()) {
      off();
    } else {
      on();
    }
    updateStatus();
  }

  void mqttStatusHandler(boolean active) {
    log("LightController", "mqtt received: " + mqtt->toString(active));
    if (active) {
      on();
    } else {
      off();
    }
  }

  void off() {
    light->off();
    log("Light", "off");
    updateStatus();
  }

  void on() {
    light->on();
    log("Light", "on");
    updateStatus();
  }
};
#endif
