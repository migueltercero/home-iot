#ifndef RollerShutterNode_H
#define RollerShutterNode_H

#include <Homie.h>


#define NODE_COMAND "command"
#define NODE_POSITION  "position"

extern "C" {
  typedef std::function<void(String)> PositionCallback;
  typedef std::function<void(String)> CommandCallback;
}

class RollerShutterNode : public HomieNode {
  protected:
    PositionCallback positionCallback;
    CommandCallback commandCallback;

    virtual bool handleInput(const String& property, const HomieRange& range, const String& value);

 public:
   RollerShutterNode(const String& id);

   void attachPosition(PositionCallback function);
   void attachCommand(CommandCallback function);

   void setPosition(String value);
};

#endif
