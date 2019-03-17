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

public:
  LightNode(const int id, const int buttonPin, const int lightPin)
      : Node("light", "Light Node", "Switch", "power", "Power ON/OFF", "boolean", "", ""), buttonPin(buttonPin), lightPin(lightPin) {}

protected:
  void setup() {
    Node::setup();

    light = new Light(lightPin);
    button = new OneButton(buttonPin, true);

    button->attachClick(bind(&LightNode::buttonClickCallback, this));
    this->attachProperty(bind(&LightNode::mqttCallback, this, _1));
  }

  void loop() {
    Node::loop();
    button->tick();
  }

  void onReadyToOperate() {
    String status = light->isActive() ? "true" : "false";
    this->send(status);
  }

  void buttonClickCallback() {
    log.info("click");
    if (light->isActive()) {
      off();
    } else {
      on();
    }
  }

  void mqttCallback(String value) {
    boolean power = (value == "true");
    if (power) {
      on();
    } else {
      off();
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
