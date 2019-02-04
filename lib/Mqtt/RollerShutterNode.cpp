#include "RollerShutterNode.h"


RollerShutterNode::RollerShutterNode(const String& id): HomieNode("rollershutter", "RollerShutter", "RollerShutter"){
  this->advertise(CHANNEL_LEVEL_ID)
      .setName(CHANNEL_LEVEL_DESCRIPTION)
      .setDatatype("integer")
      .setFormat("0:115")
      .settable();

}

void RollerShutterNode::attachLevel(LevelCallback function){
 this->levelCallback = function;
}


bool RollerShutterNode::handleInput(const HomieRange& range, const String& property, const String& value) {
  if (property.compareTo(CHANNEL_LEVEL_ID) == 0){
    this->levelCallback(value);
    return true;
  }

}

void RollerShutterNode::setLevel(String value){
  this->setProperty(CHANNEL_LEVEL_ID).send(value);
}
