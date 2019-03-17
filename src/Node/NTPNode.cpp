#ifndef NTPNode_H
#define NTPNode_H

#include "Node.cpp"
#include "TimeLib.h"
#include "TimedAction.h"
#include <NTPClient.h>
#include <WiFiUdp.h>

class NTPNode : public Node<NTPNode> {
private:
  WiFiUDP ntpUDP;
  NTPClient* ntpClient;

public:
  NTPNode() : Node("ntp", "NTP Node", "time", "time", "Time", "string", "", "") {}

protected:
  void setup() {
    Node::setup();
    
    ntpClient = new NTPClient(ntpUDP, "europe.pool.ntp.org", 0, 60 * 1000);
    ntpClient->begin();
  }
  void loop() {
    Node::loop();
    ntpClient->update();
  }
  void onReadyToOperate() {
    Node::onReadyToOperate();
    setTime(ntpClient->getEpochTime());
     send(log.getFormattedTime());
  }
};

#endif
