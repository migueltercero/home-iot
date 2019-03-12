#ifndef RollerShutterNode_H
#define RollerShutterNode_H

#include <Homie.h>

#define CHANNEL_LEVEL_ID "level"
#define CHANNEL_LEVEL_DESCRIPTION "Level"

extern "C" {
  typedef std::function<void(String)> LevelCallback;
}

class RollerShutterNode : public HomieNode {
  protected:
    LevelCallback levelCallback;
   

    virtual bool handleInput(const HomieRange& range, const String& property, const String& value);

 public:
   RollerShutterNode(const String& id);

   void attachLevel(LevelCallback function);

   void setLevel(String value);
};

#endif
