#include "LightNode.h"

LightNode::LightNode(const String& id): HomieNode("light", "Light", "Light"){
  this->advertise(CHANNEL_POWER_ID)
    .settable()
    .setName(CHANNEL_POWER_DESCRIPTION)
    .setDatatype("boolean");
}

bool LightNode::handleInput(const HomieRange& range, const String& property, const String& value) {
  if (value != "true" && value != "false") return false;

  bool status = (value == "true"); 
  this->_callbackStatus(status);
  
  return true;
}

void LightNode::attachStatus(callbackStatus function){
 this->_callbackStatus = function;
}

void LightNode::setStatus(bool on){
  String status = on ? "true" : "false";
  this->setProperty(CHANNEL_POWER_ID).send(status);
}
