#include "RollerShutterNode.h"


RollerShutterNode::RollerShutterNode(const String& id): HomieNode(id.c_str(), "RollerShutter"){
  this->advertise(NODE_POSITION).settable();
  this->advertise(NODE_COMAND).settable();
}

void RollerShutterNode::attachPosition(PositionCallback function){
 this->positionCallback = function;
}

void RollerShutterNode::attachCommand(CommandCallback function){
 this->commandCallback = function;
}

bool RollerShutterNode::handleInput(const String& property, const HomieRange& range, const String& value) {
  if (property.compareTo(NODE_POSITION) == 0){
    this->positionCallback(value);
    return true;
  }

  if (property.compareTo(NODE_COMAND) == 0){
    this->commandCallback(value);
    return true;
    }
}

void RollerShutterNode::setPosition(String value){
  this->setProperty(NODE_POSITION).send(value);
}
