#ifndef Node_H
#define Node_H

#include "Log/Logger.cpp"
#include "TimedAction.h"
#include <Homie.h>

using namespace std;

template <typename T> class Node : public HomieNode {
private:
  TimedAction* timer;

public:
  Node(const char* nodeId, const char* nodeName, const char* nodeType) : HomieNode(nodeId, nodeName, nodeType), propertyId(propertyId) {
    this->setRunLoopDisconnected(true);
  }

protected:
  Logger<T> log;
  void setup() { timer = new TimedAction(1 * 60 * 1000, bind(&Node::onReadyToOperate, this)); }
  void loop() { timer->check(); }

  bool handleInput(const HomieRange& range, const String& property, const String& value) {
    log.info("mqtt ---- [" + value + "] ----> device");
  }

  void send(const String& property, String value) {
    if (Homie.isConnected()) {
      log.info("device ---- [" + value + "] ----> mqtt");
      this->setProperty(property).send(value);
    }
  }
};

#endif
