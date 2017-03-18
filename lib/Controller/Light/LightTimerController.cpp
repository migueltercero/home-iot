#include "LightTimerController.h"

LightTimerController::LightTimerController(const String& id, const int lightPin, const int buttonPin, const int seconds)
: LightController(id, lightPin, buttonPin)
, timer(seconds*1000, bind(&LightTimerController::timerHandler, this))
{
  button->attachClick(bind(&LightTimerController::buttonClickHandler, this));
  mqtt->attachStatus(bind(&LightTimerController::mqttStatusHandler, this, _1));
}

void LightTimerController::buttonClickHandler(){
  if (!light->isActive()){
    this->on();
  }
  this->timer.enable();
}

void LightTimerController::mqttStatusHandler(bool active){
  if(active){
    this->on();
  } else {
    this->off();
  }
}

void LightTimerController::timerHandler(){
  this->off();
}

void LightTimerController::off(){
  LightController::off();
  this->timer.disable();
}

void LightTimerController::on(){
  LightController::on();
  this->timer.enable();
}


void LightTimerController::loop(){
  LightController::loop();
  this->timer.check();
}