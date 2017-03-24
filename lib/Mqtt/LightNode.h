#ifndef LightNode_H
#define LightNode_H

#include <Homie.h>


#define NODE_STATUS "status"

extern "C" {
  typedef std::function<void(bool)> callbackStatus;
}

class LightNode : public HomieNode {
  protected:
    enum Status : unsigned char { ON, OFF };

    const char* getStatus(enum Status status);

    callbackStatus _callbackFunction;

    virtual bool handleInput(const String& property, const HomieRange& range, const String& value);


 public:
   LightNode(const String& id);

   void attachStatus(callbackStatus function);

   void setStatus(bool on);
};

#endif
