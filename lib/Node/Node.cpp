#ifndef Controller_H
#define Controller_H

#include "Logger.cpp"
#include "TimedAction.h"
#include <Homie.h>

using namespace std;

class Node : public HomieNode {
private:
  const char* propertyId;
  std::function<void(String)> propertyCallback;
  TimedAction* timer;

public:
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
  void setup() { timer = new TimedAction(1 * 60 * 1000, bind(&Node::onReadyToOperate, this)); }
  void loop() { timer->check(); }

  bool handleInput(const HomieRange& range, const String& property, const String& value) {
    log(getLoggerName(), "mqtt ---- [" + value + "] ----> device");
    HomieInternals::Property* props = this->getProperty(property);

    if (property != NULL) {
      this->propertyCallback(value);
      return true;
    }
    return false;
  }
  void attachProperty(std::function<void(String)> function) { this->propertyCallback = function; }

  void send(String value) {
    if (Homie.isConnected()) {
      log(getLoggerName(), "device ---- [" + value + "] ----> mqtt");
      this->setProperty(this->propertyId).send(value);
    }
  }

  virtual String getLoggerName() = 0;
};

#endif
