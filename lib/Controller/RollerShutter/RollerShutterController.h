#ifndef RollerShutterController_H
#define RollerShutterController_H

#include "Controller.h"

using namespace std;
using namespace std::placeholders;

class RollerShutterController : public Controller{
  protected:
    int _upShutterPin;
    int _downShutterPin;

    Shutters *shutter;
    RollerShutterNode *mqtt;
    OneButton *upButton;
    OneButton *downButton;
    TimedAction *timer;

    // shutter handler
    void output(uint8_t up, uint8_t down);
    void upHandler();  
    void downHandler();  
    void haltHandler();
    uint8_t getStateHandler();
    void setStateHandler(uint8_t);
    void onLevelReachedHandler(uint8_t);

    // time handlers
    void timerHandler();

    // button handlers
    void upButtonClickHandler();
    void upButtonLongPressStartHandler();
    void upButtonDoubleClickHandler();
    void downButtonClickHandler();
    void downButtonLongPressStartHandler();
    void buttonLongPressStopHandler();

    // rollerShuter handlers
    void rollerShutterStatusHandler(String value);

    // mqtt handlers
    void mqttPositionHandler(String value);
    void mqttCommandHandler(String value);
    void updateStatus();

    void log(String text);

  public:
    RollerShutterController(const String& id, const int upButtonPin, const int downButtonPin, const int upShutterPin, const int downShutterPin, const int seconds);
    void loop();
};

#endif
