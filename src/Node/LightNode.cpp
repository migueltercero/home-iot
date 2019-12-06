#ifndef LightController_H
#define LightController_H

#include "Controller/Light.cpp"
#include "Log/Logger.cpp"
#include "Node.cpp"
#include "OneButton.h"

using namespace std;
using namespace std::placeholders;

class LightNode : public Node<LightNode> {
private:
  const int buttonPin;
  const int lightPin;

  Light* light;
  OneButton* button;

  const char* power = "power";

public:
  LightNode(const int id, const int buttonPin, const int lightPin)
      : Node("light", "Light Node", "Switch"), buttonPin(buttonPin), lightPin(lightPin) {
        light = new Light(lightPin);
    button = new OneButton(buttonPin, true);
      }

protected:
  void setup() {
    Node::setup();

    button->attachClick(bind(&LightNode::buttonClickCallback, this));
    this->advertise(power).setName("Power ON/OFF").setDatatype("boolean").settable();  
  }

  void loop() {
    Node::loop();
    button->tick();
  }

  void onReadyToOperate() {
    Node::onReadyToOperate();
    String status = light->isActive() ? "true" : "false";
    this->send(power, status);
  }

  bool handleInput(const HomieRange& range, const String& property, const String& value) {
    Node::handleInput(range, property, value);

    if (property == power) {
      boolean power = (value == "true");
      if (power) {
        on();
      } else {
        off();
      }
      return true;
    }
    return false;
  }

  void buttonClickCallback() {
    log.info("click");
    if (light->isActive()) {
      off();
    } else {
      on();
    }
  }

  void off() {
    light->off();
    log.info("off");
    onReadyToOperate();
  }

  void on() {
    light->on();
    log.info("on");
    onReadyToOperate();
  }
};
#endif
