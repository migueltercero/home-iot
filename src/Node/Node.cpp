#ifndef Node_H
#define Node_H

#include "Log/Logger.cpp"
#include "TimedAction.h"
#include <Homie.h>

using namespace std;

template <typename T> class Node : public HomieNode {
private:
  const char* propertyId;
  std::function<void(String)> propertyCallback;
  TimedAction* timer;

public:
  Node(const char* nodeId, const char* nodeName, const char* nodeType) : HomieNode(nodeId, nodeName, nodeType), propertyId(propertyId) {}

  Node(const char* nodeId, const char* nodeName, const char* nodeType, const char* propertyId, const char* propertyName,
       const char* propertyDataType, const char* propertyFormat, const char* propertyUnit)
      : HomieNode(nodeId, nodeName, nodeType), propertyId(propertyId) {

    this->advertise(propertyId)
        .setName(propertyName)
        .setDatatype(propertyDataType)
        .setFormat(propertyFormat)
        .setUnit(propertyUnit)
        .settable();

    this->setRunLoopDisconnected(true);
  }

protected:
  Logger<T> log;
  void setup() { timer = new TimedAction(1 * 60 * 1000, bind(&Node::onReadyToOperate, this)); }
  void loop() { timer->check(); }

  bool handleInput(const HomieRange& range, const String& property, const String& value) {
    log.info("mqtt ---- [" + value + "] ----> device");
    HomieInternals::Property* props = this->getProperty(property);

    if (props != NULL) {
      this->propertyCallback(value);
      return true;
    }
    return false;
  }
  void attachProperty(std::function<void(String)> function) { this->propertyCallback = function; }

  void send(String value) {
    if (Homie.isConnected()) {
      log.info("device ---- [" + value + "] ----> mqtt");
      this->setProperty(this->propertyId).send(value);
    }
  }
};

#endif
