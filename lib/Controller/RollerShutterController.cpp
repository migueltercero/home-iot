#ifndef RollerShutterController_H
#define RollerShutterController_H

#include "Controller.cpp"
#include "OneButton.h"
#include "RollerShutterNode.cpp"

#include <Shutters.h>
#include <EEPROM.h>

using namespace std;
using namespace std::placeholders;

class RollerShutterController : public Controller {
public:
  RollerShutterController(const int id, const int upButtonPin,
                          const int downButtonPin, const int upShutterPin,
                          const int downShutterPin, const int seconds)
      : Controller(id), _upShutterPin(upShutterPin),
        _downShutterPin(downShutterPin) {
    // EEPROM
    EEPROM.begin(4);

    // gpio
    pinMode(upShutterPin, OUTPUT);
    pinMode(downShutterPin, OUTPUT);
    digitalWrite(upShutterPin, LOW);
    digitalWrite(downShutterPin, LOW);

    shutter = new Shutters(
        seconds * 1000, bind(&RollerShutterController::upHandler, this),
        bind(&RollerShutterController::downHandler, this),
        bind(&RollerShutterController::haltHandler, this),
        bind(&RollerShutterController::getStateHandler, this),
        bind(&RollerShutterController::setStateHandler, this, _1),
        bind(&RollerShutterController::onLevelReachedHandler, this, _1), 0.1);
    shutter->begin();

    mqtt = new RollerShutterNode(name);
    upButton = new OneButton(upButtonPin, true);
    downButton = new OneButton(downButtonPin, true);
    
    upButton->attachClick(
        bind(&RollerShutterController::upButtonClickHandler, this));
    upButton->attachLongPressStart(
        bind(&RollerShutterController::upButtonLongPressStartHandler, this));
    upButton->attachLongPressStop(
        bind(&RollerShutterController::buttonLongPressStopHandler, this));
    // upButton->attachDoubleClick(bind(&RollerShutterController::upButtonDoubleClickHandler,
    // this));

    downButton->attachClick(
        bind(&RollerShutterController::downButtonClickHandler, this));
    downButton->attachLongPressStart(
        bind(&RollerShutterController::downButtonLongPressStartHandler, this));
    downButton->attachLongPressStop(
        bind(&RollerShutterController::buttonLongPressStopHandler, this));

    mqtt->attachProperty(
        bind(&RollerShutterController::mqttLevelHandler, this, _1));
  }

  void loop() {
    Controller::loop();
    
    upButton->tick();
    downButton->tick();

    shutter->loop();
  }

  void updateStatus() {
    String value = String(shutter->getCurrentLevel());
    mqtt->send(value);
    log("RollerShutterController", "mqtt update: " + value);
  }

protected:
  int _upShutterPin;
  int _downShutterPin;

  Shutters *shutter;
  RollerShutterNode *mqtt;
  OneButton *upButton;
  OneButton *downButton;

  // shutter handler
  void output(uint8_t up, uint8_t down) {
    // nunca activar ambas salidas
    if (up == HIGH && down == HIGH) {
      return;
    }

    // ouput
    digitalWrite(_upShutterPin, up);
    digitalWrite(_downShutterPin, down);
  }

  void upHandler() {
    log("RollerShutterController", "up");
    output(HIGH, LOW);
  }

  void downHandler() {
    log("RollerShutterController", "down");
    output(LOW, HIGH);
  }

  void haltHandler() {
    log("RollerShutterController", "stop");
    output(LOW, LOW);
  }

  uint8_t getStateHandler() {
    uint8_t level = EEPROM.read(0);
    log("RollerShutterController", "EEPROM -> " + String(level));
    if (level >= ShuttersInternal::UP_LEVEL &&
        level <= ShuttersInternal::DOWN_LEVEL) {
      return level;
    } else {
      return 255;
    }
  }

  void setStateHandler(uint8_t level) {
    if (level != 255) {
      EEPROM.write(0, level);
      EEPROM.commit();
      log("RollerShutterController", "EEPROM <- " + String(level));
    }
  }

  void onLevelReachedHandler(uint8_t level) {
    log("RollerShutterController", "level " + String(level));
    if (level % 10 == 0) {
      updateStatus();
    }
  }

  // button handlers
  void upButtonClickHandler() {
    if (shutter->isIdle()) {
      shutter->setLevel(ShuttersInternal::UP_LEVEL);
      updateStatus();
    } else {
      shutter->stop();
    }
  }

  void upButtonDoubleClickHandler() {
    shutter->setLevel((100 - ShuttersInternal::UP_LEVEL) / 2);
  }

  void upButtonLongPressStartHandler() { upButtonClickHandler(); }

  void buttonLongPressStopHandler() { shutter->stop(); }

  void downButtonClickHandler() {
    if (shutter->isIdle()) {
      shutter->setLevel(ShuttersInternal::DOWN_LEVEL);
      updateStatus();
    } else {
      shutter->stop();
    }
  }

  void downButtonLongPressStartHandler() { downButtonClickHandler(); }

  // rollerShuter handlers
  void rollerShutterStatusHandler(String value) { updateStatus(); }

  // mqtt handlers
  void mqttLevelHandler(String value) {
    log("RollerShutterController", "mqtt received: " + value);
    int level = value.toInt();
    if (level == 255) {
      shutter->stop();
    } else {
      shutter->setLevel(level);
    }
  }

  
};
#endif
