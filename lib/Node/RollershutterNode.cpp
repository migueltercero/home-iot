#ifndef Rollershutter_H
#define Rollershutter_H

#include "EEPROM.h"
#include "Logger.cpp"
#include "Node.cpp"
#include "OneButton.h"
#include "Shutters.h"

using namespace std;
using namespace std::placeholders;

class RollershutterNode : public Node {
private:
  const int upButtonPin;
  const int downButtonPin;
  const int upShutterPin;
  const int downShutterPin;
  const int seconds;

  Shutters* shutter;
  OneButton* upButton;
  OneButton* downButton;

public:
  RollershutterNode(const int id, const int upButtonPin, const int downButtonPin, const int upShutterPin, const int downShutterPin,
                    const int seconds)
      : Node("rollershutter", "Rollershutter Node", "Rollershutter", "level", "Level", "integer", "0:100", "%"), upButtonPin(upButtonPin),
        downButtonPin(downButtonPin), upShutterPin(upShutterPin), downShutterPin(downShutterPin), seconds(seconds) {}

protected:
  void setup() {
    Node::setup();

    // EEPROM
    EEPROM.begin(4);

    // gpio
    pinMode(upShutterPin, OUTPUT);
    pinMode(downShutterPin, OUTPUT);
    digitalWrite(upShutterPin, LOW);
    digitalWrite(downShutterPin, LOW);

    shutter =
        new Shutters(seconds * 1000, bind(&RollershutterNode::upCallback, this), bind(&RollershutterNode::downCallback, this),
                     bind(&RollershutterNode::haltCallback, this), bind(&RollershutterNode::getLevelCallback, this),
                     bind(&RollershutterNode::setLevelCallback, this, _1), bind(&RollershutterNode::onLevelReachedCallback, this, _1), 0.1);
    shutter->begin();

    upButton = new OneButton(upButtonPin, true);
    downButton = new OneButton(downButtonPin, true);

    upButton->attachClick(bind(&RollershutterNode::buttonUpClickCallback, this));
    upButton->attachLongPressStart(bind(&RollershutterNode::buttonUpLongPressStartCallback, this));
    upButton->attachLongPressStop(bind(&RollershutterNode::buttonLongPressStopCallback, this));
    // upButton->attachDoubleClick(bind(&RollerShutterController::upButtonDoubleClickCallback,
    // this));

    downButton->attachClick(bind(&RollershutterNode::buttonDownClickCallback, this));
    downButton->attachLongPressStart(bind(&RollershutterNode::buttonDownLongPressStartCallback, this));
    downButton->attachLongPressStop(bind(&RollershutterNode::buttonLongPressStopCallback, this));

    this->attachProperty(bind(&RollershutterNode::mqttCallback, this, _1));
  }

  void loop() {
    Node::loop();

    upButton->tick();
    downButton->tick();
    shutter->loop();
  }

  void onReadyToOperate() {
    String status = String(shutter->getCurrentLevel());
    this->send(status);
  }

  String getLoggerName() { return "RollershutterNode"; }

  void output(uint8_t up, uint8_t down) {
    if (up == HIGH && down == HIGH) {
      return;
    }

    digitalWrite(upShutterPin, up);
    digitalWrite(downShutterPin, down);
  }

  void upCallback() {
    log(getLoggerName(), "up");
    output(HIGH, LOW);
  }

  void downCallback() {
    log(getLoggerName(), "down");
    output(LOW, HIGH);
  }

  void haltCallback() {
    log(getLoggerName(), "stop");
    output(LOW, LOW);
  }

  uint8_t getLevelCallback() {
    uint8_t level = EEPROM.read(0);
    log(getLoggerName(), "EEPROM -> " + String(level));
    if (level >= ShuttersInternal::UP_LEVEL && level <= ShuttersInternal::DOWN_LEVEL) {
      return level;
    } else {
      return 255;
    }
  }

  void setLevelCallback(uint8_t level) {
    if (level != 255) {
      EEPROM.write(0, level);
      EEPROM.commit();
      log(getLoggerName(), "EEPROM <- " + String(level));
    }
  }

  void onLevelReachedCallback(uint8_t level) {
    log(getLoggerName(), "level " + String(level));
    if (level % 10 == 0) {
      onReadyToOperate();
    }
  }

  void buttonUpClickCallback() {
    if (shutter->isIdle()) {
      shutter->setLevel(ShuttersInternal::UP_LEVEL);
      onReadyToOperate();
    } else {
      shutter->stop();
    }
  }

  void buttonUpDoubleClickCallback() { shutter->setLevel((100 - ShuttersInternal::UP_LEVEL) / 2); }

  void buttonUpLongPressStartCallback() { buttonUpClickCallback(); }

  void buttonLongPressStopCallback() { shutter->stop(); }

  void buttonDownClickCallback() {
    if (shutter->isIdle()) {
      shutter->setLevel(ShuttersInternal::DOWN_LEVEL);
      onReadyToOperate();
    } else {
      shutter->stop();
    }
  }

  void buttonDownLongPressStartCallback() { buttonDownClickCallback(); }

  // rollerShuter Callbacks
  void rollerShutterStatusCallback(String value) { onReadyToOperate(); }

  void mqttCallback(String value) {
    int level = value.toInt();
    if (level == 255) {
      shutter->stop();
    } else {
      shutter->setLevel(level);
    }
  }
};
#endif
