#include "RollerShutterController.h"
#include "TimedAction.h"

RollerShutterController::RollerShutterController(const int id, const int upButtonPin, const int downButtonPin, const int upShutterPin, const int downShutterPin, const int seconds) : Controller(id)
,_upShutterPin(upShutterPin)
,_downShutterPin(downShutterPin)
{
  // EEPROM
  EEPROM.begin(4);
    
  // gpio
  pinMode(upShutterPin, OUTPUT);
  pinMode(downShutterPin, OUTPUT);
  digitalWrite(upShutterPin, LOW);
  digitalWrite(downShutterPin, LOW);

  shutter = new Shutters(seconds*1000, 
    bind(&RollerShutterController::upHandler, this), 
    bind(&RollerShutterController::downHandler, this), 
    bind(&RollerShutterController::haltHandler, this), 
    bind(&RollerShutterController::getStateHandler, this),
    bind(&RollerShutterController::setStateHandler, this, _1), 
    bind(&RollerShutterController::onLevelReachedHandler, this, _1),
    0.1);
  shutter->begin();

  mqtt = new RollerShutterNode(name);
  upButton = new OneButton(upButtonPin, true);
  downButton = new OneButton(downButtonPin, true);
  timer = new TimedAction(1*60*1000, bind(&RollerShutterController::timerHandler, this));

  upButton->attachClick(bind(&RollerShutterController::upButtonClickHandler, this));
  upButton->attachLongPressStart(bind(&RollerShutterController::upButtonLongPressStartHandler, this));
  upButton->attachLongPressStop(bind(&RollerShutterController::buttonLongPressStopHandler, this));
  //upButton->attachDoubleClick(bind(&RollerShutterController::upButtonDoubleClickHandler, this));
  
  downButton->attachClick(bind(&RollerShutterController::downButtonClickHandler, this));
  downButton->attachLongPressStart(bind(&RollerShutterController::downButtonLongPressStartHandler, this));
  downButton->attachLongPressStop(bind(&RollerShutterController::buttonLongPressStopHandler, this));

  mqtt->attachLevel(bind(&RollerShutterController::mqttLevelHandler, this, _1));
  
}

// shutters handler
void RollerShutterController::output(uint8_t up, uint8_t down){
  // nunca activar ambas salidas
  if (up == HIGH && down == HIGH){
    return;
  }

  //ouput
  digitalWrite(_upShutterPin, up);
  digitalWrite(_downShutterPin, down);
}

void RollerShutterController::upHandler(){
  log("up");
  output(HIGH, LOW);
}

void RollerShutterController::downHandler(){
  log("down");
  output(LOW, HIGH);
}

void RollerShutterController::haltHandler(){
  log("stop");
  output(LOW, LOW);
}

uint8_t RollerShutterController::getStateHandler(){
  uint8_t level = EEPROM.read(0);
  log("EEPROM -> "+String(level));
  if (level >= ShuttersInternal::UP_LEVEL && level <= ShuttersInternal::DOWN_LEVEL){
    return level;
  } else {
    return 255;
  }
}

void RollerShutterController::setStateHandler(uint8_t level){
  if (level != 255){
    EEPROM.write(0, level);
    EEPROM.commit();
    log("EEPROM <- "+String(level));
  }
}

void RollerShutterController::onLevelReachedHandler(uint8_t level){
   log("level "+String(level));
   if (level % 10 == 0){
     updateStatus();
   }
}

//timer handler
void RollerShutterController::timerHandler() {
  updateStatus();
}

// mqtt events
void RollerShutterController::mqttLevelHandler(String value) {
  shutter->setLevel(value.toInt());
}

void RollerShutterController::updateStatus() {
  String value = String(shutter->getCurrentLevel());
  log("mqtt <- " + value);
  mqtt->setLevel(value);
}


// rollershutter handlers
void RollerShutterController::rollerShutterStatusHandler(String value)
{
  updateStatus();
}

// button handlers
void RollerShutterController::upButtonClickHandler(){
  if (shutter->isIdle()){
    shutter->setLevel(ShuttersInternal::UP_LEVEL);
    updateStatus();  
  } else {
    shutter->stop();
  }
}

void RollerShutterController::upButtonDoubleClickHandler(){
  shutter->setLevel((100 - ShuttersInternal::UP_LEVEL)/2);
}

void RollerShutterController::upButtonLongPressStartHandler(){
  upButtonClickHandler();
}

void RollerShutterController::buttonLongPressStopHandler(){
  shutter->stop();
}

void RollerShutterController::downButtonClickHandler(){
  if (shutter->isIdle()){
    shutter->setLevel(ShuttersInternal::DOWN_LEVEL);
    updateStatus();  
  } else {
    shutter->stop();
  }
}

void RollerShutterController::downButtonLongPressStartHandler(){
  downButtonClickHandler();
}

// loop
void RollerShutterController::loop() {
  upButton->tick();
  downButton->tick();

  shutter->loop();

  timer->check();
}

void RollerShutterController::log(String text) {
  Homie.getLogger() << "[Shutters] " << (const char*)text.c_str() << endl;
}
