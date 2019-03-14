#include "Node.cpp"

#define NODE_ID "rollershutter"
#define NODE_NAME "Roller Shutter Controller"
#define NODE_TYPE "Rollershutter"
#define PROPERTY_ID "level"
#define PROPERTY_NAME "Level"
#define PROPERTY_DATATYPE "integer"
#define PROPERTY_FORMAT "0:100"
#define PROPERTY_UNIT "%"

class RollerShutterNode : public Node<String> {
public:
  RollerShutterNode(const String &id)
      : Node(NODE_ID, NODE_NAME, NODE_TYPE, PROPERTY_ID, PROPERTY_NAME,
             PROPERTY_DATATYPE, PROPERTY_FORMAT, PROPERTY_UNIT) {}

  String fromString(String value) { return value; }

  String toString(String value) { return value; }
};