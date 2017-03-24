#ifndef RollerShutterController_H
#define RollerShutterController_H

#include "Controller.h"

using namespace std;
using namespace std::placeholders;

class RollerShutterController : public Controller{
  protected:
    Shutters *shutter;
    RollerShutterNode *mqtt;
    OneButton *upButton;
    OneButton *downButton;

    // button handlers
    void upButtonClickHandler();
    void upButtonLongPressStartHandler();
    void downButtonClickHandler();
    void downButtonLongPressStartHandler();
    void buttonLongPressStopHandler();

    // rollerShuter handlers
    void rollerShutterStatusHandler(String value);

    // mqtt handlers
    void mqttPositionHandler(String value);
    void mqttCommandHandler(String value);

  public:
    RollerShutterController(const String& id, const int upButtonPin, const int downButtonPin, const int upShutterPin, const int downShutterPin, const int seconds);
    void loop();
};

#endif
