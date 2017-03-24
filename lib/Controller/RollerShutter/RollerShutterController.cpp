#include "RollerShutterController.h"

#define UP_LEVEL 15
#define DOWN_LEVEL 115

RollerShutterController::RollerShutterController(const String& id, const int upButtonPin, const int downButtonPin, const int upShutterPin, const int downShutterPin, const int seconds) : Controller(id)
{
  shutter = new Shutters(upShutterPin, downShutterPin, seconds, DOWN_LEVEL);
  mqtt = new RollerShutterNode(name);
  upButton = new OneButton(upButtonPin, true);
  downButton = new OneButton(downButtonPin, true);


  shutter->attachStatus(bind(&RollerShutterController::rollerShutterStatusHandler, this, _1));

  upButton->attachClick(bind(&RollerShutterController::upButtonClickHandler, this));
  upButton->attachLongPressStart(bind(&RollerShutterController::upButtonLongPressStartHandler, this));
  upButton->attachLongPressStop(bind(&RollerShutterController::buttonLongPressStopHandler, this));
  //upButton->setClickTicks(100);
  //upButton->setPressTicks(500);
  downButton->attachClick(bind(&RollerShutterController::downButtonClickHandler, this));
  downButton->attachLongPressStart(bind(&RollerShutterController::downButtonLongPressStartHandler, this));
  downButton->attachLongPressStop(bind(&RollerShutterController::buttonLongPressStopHandler, this));

  mqtt->attachPosition(bind(&RollerShutterController::mqttPositionHandler, this, _1));
  mqtt->attachCommand(bind(&RollerShutterController::mqttCommandHandler, this, _1));

}


// mqtt events
void RollerShutterController::mqttPositionHandler(String value) {
  shutter->requestLevel(value.toInt());
}

void RollerShutterController::mqttCommandHandler(String value) {
  if (value.compareTo("STOP") == 0){
    shutter->stop();
  }
  if (value.compareTo("UP") == 0){
    shutter->requestLevel(UP_LEVEL);
  }
  if (value.compareTo("DOWN") == 0){
    shutter->requestLevel(DOWN_LEVEL);
  }
}


// rollershutter handlers
void RollerShutterController::rollerShutterStatusHandler(String value)
{
  mqtt->setPosition(value);
}

// button handlers
void RollerShutterController::upButtonClickHandler(){
  if (shutter->moving()){
    shutter->stop();
  } else {
      shutter->requestLevel(UP_LEVEL);
  }
}

void RollerShutterController::upButtonLongPressStartHandler(){
  if (shutter->moving()){
    shutter->stop();
  } else {
    shutter->up();
  }
}

void RollerShutterController::buttonLongPressStopHandler(){
  shutter->stop();
}

void RollerShutterController::downButtonClickHandler(){
  if (shutter->moving()){
    shutter->stop();
  } else {
      shutter->requestLevel(DOWN_LEVEL);
  }
}

void RollerShutterController::downButtonLongPressStartHandler(){
  if (shutter->moving()){
    shutter->stop();
  } else {
    shutter->down();
  }
}

// loop
void RollerShutterController::loop() {
  upButton->tick();
  downButton->tick();

  shutter->loop();
}
