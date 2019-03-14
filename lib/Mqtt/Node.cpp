#ifndef Node_H
#define Node_H

#include <Homie.h>

template <class T> class Node : public HomieNode {
public:
  Node(const char *nodeId, const char *nodeName, const char *nodeType,
       const char *propertyId, const char *propertyName,
       const char *propertyDataType, const char *propertyFormat,
       const char *propertyUnit)
      : HomieNode(nodeId, nodeName, nodeType), propertyId(propertyId) {
    this->advertise(propertyId)
        .setName(propertyName)
        .setDatatype(propertyDataType)
        .setFormat(propertyFormat)
        .setUnit(propertyUnit)
        .settable();
  }

  void send(T value) {
    if (Homie.isConnected()) {
      this->setProperty(this->propertyId).send(toString(value));
    }
  }

  void attachProperty(std::function<void(T)> function) {
    this->propertyCallback = function;
  }

protected:
  virtual T fromString(String value) = 0;

  virtual String toString(T value) = 0;

private:
  std::function<void(T)> propertyCallback;
  const char *propertyId;

  bool handleInput(const HomieRange &range, const String &property,
                   const String &value) {
    if (property.compareTo(propertyId) == 0) {
      this->propertyCallback(fromString(value));
      return true;
    }
    return false;
  }
};
#endif