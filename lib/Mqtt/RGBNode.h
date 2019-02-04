#ifndef RGBNode_H
#define RGBNode_H

#include <LightNode.h>

#define NODE_COLOR "color"

extern "C" {
  typedef std::function<void(bool)> callbackColor;
}

class RGBNode : public LightNode {
  protected:
    callbackColor _callbackColor;

  public:
   RGBNode(const String& id);

   void attachColor(callbackColor function);

   void setColor(int r, int g, int b);
};

#endif
