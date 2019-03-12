#ifndef LightTimerController_H
#define LightTimerController_H

#include "LightController.h"
#include "TimedAction.h"

class LightTimerController : public LightController {
  protected:
    TimedAction timer;

    void buttonClickHandler();

    void mqttStatusHandler(bool active);

    void on();

    void off();

    void timerHandler();

  public:
    LightTimerController(const int id, const int buttonPin, const int lightPin,const int seconds);
    void loop();
    void updateStatus();
};
#endif
