#include "Node.cpp"

class LightNode : public Node<boolean> {
public:
  LightNode(const String &id)
      : Node("light", "Light Controller", "Switch", "power", "Power ON/OFF",
             "boolean", "", "") {}

  boolean fromString(String value) { return (value == "true"); }

  String toString(boolean value) { return value ? "true" : "false"; }

};