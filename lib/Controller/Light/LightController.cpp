#include "LightController.h"

LightController::LightController(const int id, int buttonPin, int lightPin) : Controller(id)
{
  light = new Light(lightPin);
  mqtt = new LightNode(name);
  button = new OneButton(buttonPin, true);

  button->attachClick(bind(&LightController::buttonClickHandler, this));
  mqtt->attachStatus(bind(&LightController::mqttStatusHandler, this, _1));
}

void LightController::buttonClickHandler(){
  if(light->isActive()){
    off();
  }else {
    on();
  }
}

void LightController::mqttStatusHandler(bool active){
  if(active){
    on();
  } else {
    off();
  }
}

void LightController::off(){
  light->off();
  mqtt->setStatus(false);
}

void LightController::on(){
  light->on();
  mqtt->setStatus(true);
}

void LightController::loop(){
  button->tick();
}
