#include "RGBNode.h"

RGBNode::RGBNode(const String& id): LightNode(id.c_str()){
  this->advertise(NODE_COLOR).settable();
}




void RGBNode::attachColor(callbackColor function){
 this->_callbackColor = function;
}

void RGBNode::setColor(int r, int g, int b){
  this->setProperty(NODE_COLOR).send(r+","+g);
}
