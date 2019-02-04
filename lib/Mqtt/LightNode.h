#ifndef LightNode_H
#define LightNode_H

#include <Homie.h>

#define CHANNEL_POWER_ID "power"
#define CHANNEL_POWER_DESCRIPTION "Power ON/OFF"

extern "C" {
  typedef std::function<void(bool)> callbackStatus;
}

class LightNode : public HomieNode {
  protected:
    callbackStatus _callbackStatus;
    virtual bool handleInput(const HomieRange& range, const String& property, const String& value);
 
 public:
   LightNode(const String& id);

   void attachStatus(callbackStatus function);

   void setStatus(bool on);
};

#endif
