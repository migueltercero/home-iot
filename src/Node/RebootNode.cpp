#ifndef RebootNode_H
#define RebootNode_H

#include "Node.cpp"
#include "TimedAction.h"

class RebootNode : public Node<RebootNode> {
private:
  TimedAction* timer;

  const char* enable = "enable";

  // HomieSetting<bool> reboot("reboot", "¿reboot programmed?");
  // HomieSetting<long> rebootTime("rebootTime", "Frecuency in hours for reboot");
  // HomieSetting<long> nodeCount("controllerCount", "Number of Nodes");

public:
  RebootNode() : Node("reboot", "Reboot Node", "Switch") {}

protected:
  void setup() {
    // reboot.setDefaultValue(true);
    // rebootTime.setDefaultValue(24);
    // nodeCount.setDefaultValue(0);
    this->advertise(enable).setName("Reboot").setDatatype("boolean");
    timer = new TimedAction(24 * 60 * 60 * 1000, bind(&RebootNode::timeCallback, this));
  }

  void loop() {
    timer->check();
  }

  void timeCallback() {
    log.info("reboot");
    Homie.reboot();
  }
};

#endif
