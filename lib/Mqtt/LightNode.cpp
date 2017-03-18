#include "LightNode.h"

const char* LightNode::getStatus(enum Status status)
{
  switch (status)
  {
    case ON: return "ON";
    case OFF: return "OFF";
  }
}

bool LightNode::handleInput(const String& property, const HomieRange& range, const String& value) {
  if (property.compareTo(NODE_STATUS) == 0){
    if(value.compareTo(getStatus(ON)) == 0){
      this->_callbackFunction(true);
    }
    if(value.compareTo(getStatus(OFF)) == 0){
      this->_callbackFunction(false);
    }
  }
}

LightNode::LightNode(const String& id): HomieNode(id.c_str(), NODE_DESCRIPTION){
  this->advertise(NODE_STATUS).settable();
}

void LightNode::attachStatus(callbackStatus function){
 this->_callbackFunction = function;
}

void LightNode::setStatus(bool on){
  Status status = on ? ON : OFF;
  this->setProperty(NODE_STATUS).send(getStatus(status));
}
